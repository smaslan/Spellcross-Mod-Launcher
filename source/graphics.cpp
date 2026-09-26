// Graphics routines and stuff
// based on:
//   https://indiegamedev.net/2020/01/17/median-cut-with-floyd-steinberg-dithering-in-c/
// and google Gemini AI

#include "graphics.h"
#include <algorithm>
#include <vector>
#include <queue>

// make new color bucket
ImgQuantize::Bucket::Bucket(std::vector<ImgQuantize::Pixel>::iterator s,std::vector<ImgQuantize::Pixel>::iterator e)
    : start(s),end(e)
{
    // fond range of color channels
    minR = minG = minB = 255;
    maxR = maxG = maxB = 0;
    for(auto it = start; it != end; ++it) {
        if(it->r < minR) minR = it->r;
        if(it->r > maxR) maxR = it->r;
        if(it->g < minG) minG = it->g;
        if(it->g > maxG) maxG = it->g;
        if(it->b < minB) minB = it->b;
        if(it->b > maxB) maxB = it->b;
    }
}

// get color with widest range
int ImgQuantize::Bucket::getLongestSide() const {
    int rRange = maxR - minR;
    int gRange = maxG - minG;
    int bRange = maxB - minB;
    return std::max({rRange, gRange, bRange});
}

// find palette by median cut algorithm
std::vector<ImgQuantize::Pixel> ImgQuantize::GenMedianCutPalette(std::vector<ImgQuantize::Pixel>& pixels,int targetColors)
{
    std::vector<Pixel> pal;
    if(pixels.empty())
        return(pal);

    std::vector<Bucket> buckets;
    buckets.emplace_back(pixels.begin(),pixels.end());

    // Keep splitting until we reach the target palette count
    while(buckets.size() < targetColors)
    {
        // Find the bucket with the largest overall color channel range
        auto splitTarget = buckets.end();
        int maxRange = -1;

        for(auto it = buckets.begin(); it != buckets.end(); ++it) {
            // Ensure bucket has at least 2 pixels to be physically splittable
            if(std::distance(it->start,it->end) > 1) {
                int range = it->getLongestSide();
                if(range > maxRange) {
                    maxRange = range;
                    splitTarget = it;
                }
            }
        }

        // If no more buckets can be split, break early
        if(splitTarget == buckets.end())
            break;

        // Determine which specific channel is the longest
        int rRange = splitTarget->maxR - splitTarget->minR;
        int gRange = splitTarget->maxG - splitTarget->minG;
        int bRange = splitTarget->maxB - splitTarget->minB;

        auto startIdx = splitTarget->start;
        auto endIdx = splitTarget->end;
        auto medianIdx = startIdx + std::distance(startIdx,endIdx) / 2;

        // Efficient linear-time O(N) partitioning on the longest channel
        if(rRange >= gRange && rRange >= bRange)
            std::nth_element(startIdx,medianIdx,endIdx,[](const Pixel& a,const Pixel& b) { return a.r < b.r; });
        else if(gRange >= rRange && gRange >= bRange)
            std::nth_element(startIdx,medianIdx,endIdx,[](const Pixel& a,const Pixel& b) { return a.g < b.g; });
        else
            std::nth_element(startIdx,medianIdx,endIdx,[](const Pixel& a,const Pixel& b) { return a.b < b.b; });

        // Split the target bucket into two parts around the median point
        Bucket left(startIdx,medianIdx);
        Bucket right(medianIdx,endIdx);

        // Erase old bucket, inject the two new subdivisions
        buckets.erase(splitTarget);
        buckets.push_back(left);
        buckets.push_back(right);
    }

    struct Pal {
        int r;
        int g;
        int b;
        double h;
        double s;
        double v;
        double x;
    };

    // find average colors
    std::vector<Pal> palette;
    for(const Bucket& box: buckets)
    {
        int redAccum = 0;
        int greenAccum = 0;
        int blueAccum = 0;
        std::for_each(box.start,box.end,[&](const Pixel& p)
            {
                redAccum += p.r;
                greenAccum += p.g;
                blueAccum += p.b;
            });
        int size = box.end - box.start + 1;
        if(size)
        {
            redAccum /= size;
            greenAccum /= size;
            blueAccum /= size;
        }
        Pal col;
        col.r = std::min((uint32_t)redAccum,255u);
        col.g = std::min((uint32_t)greenAccum,255u);
        col.b = std::min((uint32_t)blueAccum,255u);
        int cmax = std::max(std::max(col.r,col.g),col.b);
        int cmin = std::min(std::min(col.r,col.g),col.b);
        int delta = cmax - cmin;
        if(delta <= 0.0)
            col.h = 0.0;
        else if(cmax == col.r)
            col.h = 60.0*(col.g - col.b)/delta + 360.0;
        else if(cmax == col.g)
            col.h = 60.0*(col.b - col.r)/delta + 120.0;
        else
            col.h = 60.0*(col.r - col.g)/delta + 240.0;
        col.h = fmod(col.h + 360.0*2.0,360.0);
        if(!delta)
            col.s = 0.0;
        else
            col.s = (double)delta/cmax;
        col.v = cmax/256.0;

        //col.x = col.h*256.0*256.0 + col.s*256.0 + col.v;
        col.x = (col.h/360.0 + 1)*10 + col.v;

        palette.push_back(col);
    }


    // try to sort result to make it "pretty"
    //std::sort(palette.begin(),palette.end(),[](const Pal& a,const Pal& b) {return a.v < b.v;});
    //std::sort(palette.begin(),palette.end(),[](const Pal& a,const Pal& b) {return a.s < b.s;});
    //std::sort(palette.begin(), palette.end(),[](const Pal& a,const Pal& b) {return a.h < b.h;});
    std::ranges::sort(palette,[](const Pal& a,const Pal& b) {return a.x < b.x;});

    for(auto& col: palette)
    {
        Pixel pix(col.r,col.g,col.b);
        pal.push_back(pix);
    }

    return(pal);
}



// geometric distance of colors
int ImgQuantize::Pixel::distance_squared(const Pixel& other) const {
    int dr = static_cast<int>(r) - other.r;
    int dg = static_cast<int>(g) - other.g;
    int db = static_cast<int>(b) - other.b;
    return(dr*dr + dg*dg + db*db);
}

// geometric distance of colors
int ImgQuantize::Pixel::max_distance_linear(const Pixel& other) const {
    int dr = static_cast<int>(r) - other.r;
    int dg = static_cast<int>(g) - other.g;
    int db = static_cast<int>(b) - other.b;    
    return(std::max<int>({std::abs(dr),std::abs(dg),std::abs(db)}));
}

// is pixel black?
bool ImgQuantize::Pixel::isBlack() const {    
    return(!r && !g && !b);
}

// is pixel transparent?
bool ImgQuantize::Pixel::isTransparent() const {
    return(!a);
}







// node constructor
ImgQuantize::OctreeNode::OctreeNode(int minR,int maxR,int minG, int maxG,int minB,int maxB, int level)
    : minR(minR),maxR(maxR),minG(minG),maxG(maxG),minB(minB),maxB(maxB),level(level),is_leaf(false),leaf_color({0, 0, 0})
{
    for(int i = 0; i < 8; i++)
        children[i] = nullptr;
}

// minimum vector distance from cube edges
int ImgQuantize::OctreeNode::MinDistanceSquared(const Pixel& target) const
{
    int dr = 0, dg = 0, db = 0;

    if(target.r < minR)
        dr = minR - target.r;
    else if(target.r > maxR)
        dr = target.r - maxR;

    if(target.g < minG)
        dg = minG - target.g;
    else if(target.g > maxG)
        dg = target.g - maxG;

    if(target.b < minB)
        db = minB - target.b;
    else if(target.b > maxB)
        db = target.b - maxB;

    return(dr*dr + dg*dg + db*db);
}


// init octree
ImgQuantize::ColorOctree::ColorOctree()
{
    root = std::make_unique<OctreeNode>(0,255,0, 255,0,255, 0);
}

// Determines which of the 8 children a color belongs to at a specific bit depth
int ImgQuantize::ColorOctree::GetChildIndex(const Pixel& cls,int depth) {
    int shift = 7 - depth;
    int r_bit = (cls.r >> shift) & 1;
    int g_bit = (cls.g >> shift) & 1;
    int b_bit = (cls.b >> shift) & 1;
    return((r_bit << 2) | (g_bit << 1) | b_bit);
}

// insert color to tree
void ImgQuantize::ColorOctree::Insert(const Pixel& color,int id)
{
    OctreeNode* current = root.get();

    for(int level = 0; level < 8; ++level)
    {
        int index = GetChildIndex(color, level);
        if(!current->children[index])
        {
            // get sub-cube dims
            int midR = current->minR + (current->maxR - current->minR) / 2;
            int midG = current->minG + (current->maxG - current->minG) / 2;
            int midB = current->minB + (current->maxB - current->minB) / 2;
            int nextMinR = (index & 4) ? midR + 1 : current->minR;
            int nextMaxR = (index & 4) ? current->maxR : midR;
            int nextMinG = (index & 2) ? midG + 1 : current->minG;
            int nextMaxG = (index & 2) ? current->maxG : midG;
            int nextMinB = (index & 1) ? midB + 1 : current->minB;
            int nextMaxB = (index & 1) ? current->maxB : midB;
            current->children[index] = std::make_unique<OctreeNode>(nextMinR,nextMaxR,nextMinG, nextMaxG,nextMinB,nextMaxB, level);
        }
        current = current->children[index].get();
    }
    current->is_leaf = true;
    current->leaf_color = color;
    current->leaf_color.id = id;
    current->index = id;
}

// receoursive search color using DFS method
void ImgQuantize::ColorOctree::searchClosestDFS(const OctreeNode* node,const Pixel& target,
    const OctreeNode*& best1,int& bestDist1,
    const OctreeNode*& best2,int& bestDist2) const
{
    if(!node)
        return;

    // initial pruning
    if(node->MinDistanceSquared(target) >= bestDist2)
        return;
    
    // find 2 closest candidates
    if(node->is_leaf) {
        int d = node->leaf_color.distance_squared(target);
        if(d < bestDist1)
        {
            bestDist2 = bestDist1;
            best2 = best1;
            bestDist1 = d;
            best1 = node;
        }
        else if(d < bestDist2)
        {
            bestDist2 = d;
            best2 = node;
        }
        return;
    }

    // heuristic estimate of closest child using bit mask
    int preferredIndex = GetChildIndex(target,node->level);
    if(node->children[preferredIndex])
        searchClosestDFS(node->children[preferredIndex].get(),target,best1,bestDist1,best2,bestDist2);
    
    // check other candidates
    for(int i = 0; i < 8; ++i)
    {
        if(i != preferredIndex && node->children[i])
            searchClosestDFS(node->children[i].get(),target,best1,bestDist1,best2,bestDist2);
    }
}

// find two nearest colors
std::pair<ImgQuantize::Pixel,ImgQuantize::Pixel> ImgQuantize::ColorOctree::FindTwoNearestColors(const Pixel& target) const
{
    // initial distance to inf
    const OctreeNode* best1 = nullptr;
    const OctreeNode* best2 = nullptr;
    int bestDist1 = std::numeric_limits<int>::max();
    int bestDist2 = std::numeric_limits<int>::max();

    // resoursive search
    searchClosestDFS(root.get(),target, best1,bestDist1, best2,bestDist2);

    // ensure we always return something
    Pixel best_color(0,0,0,0,0);
    Pixel secondary_color(0,0,0,0,0);
    if(best1)
        secondary_color = best_color = best1->leaf_color;
    if(best2)
        secondary_color = best2->leaf_color;
    return(std::make_pair(best_color,secondary_color));
}


