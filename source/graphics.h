#pragma once

#include <cstdint>
#include <vector>
#include <memory>

class ImgQuantize{

public:

    class Pixel
    {
    public:
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
        int id;

        Pixel(uint8_t r=0,uint8_t g=0,uint8_t b=0,uint8_t a=0,int id=0) :
            r(r), g(g), b(b), a(a), id(id) {};
        int distance_squared(const Pixel& other) const;
        int max_distance_linear(const Pixel& other) const;
        bool isBlack() const;
        bool isTransparent() const;   
    };

    class Bucket
    {
    public:
        std::vector<Pixel>::iterator start;
        std::vector<Pixel>::iterator end;
        uint8_t minR,maxR,minG,maxG,minB,maxB;

        // make bucket from pixels
        Bucket(std::vector<Pixel>::iterator s,std::vector<Pixel>::iterator e);
        int getLongestSide() const;
    };
    
    static std::vector<Pixel> GenMedianCutPalette(std::vector<Pixel> &pixels,int targetColors);          

    // Node structure for the Octree
    class OctreeNode
    {
    public:
        int minR,maxR;
        int minG,maxG;
        int minB,maxB;
        bool is_leaf;
        int level;
        Pixel leaf_color; // Valid only if is_leaf is true
        int index;
        std::unique_ptr<OctreeNode> children[8];

        OctreeNode(int minR,int maxR,int minG, int maxG,int minB,int maxB, int level);
        int MinDistanceSquared(const Pixel& target) const;
    };

    class ColorOctree
    {
    private:
        std::unique_ptr<OctreeNode> root;

        // Determines which of the 8 children a color belongs to at a specific bit depth
        static int GetChildIndex(const Pixel& cls,int depth);

        // priority queue item
        class QueueItem
        {
        public:
            const OctreeNode* node;
            int distanceSq;
            bool operator>(const QueueItem& other) const {
                return distanceSq > other.distanceSq;
            }
        };

        void searchClosestDFS(const OctreeNode* node,const Pixel& target,
            const OctreeNode*& best1,int& bestDist1,
            const OctreeNode*& best2,int& bestDist2) const;

    public:
        ColorOctree();
        void Insert(const Pixel& color,int id);
        std::pair<Pixel,Pixel> FindTwoNearestColors(const Pixel& target) const;
    };

private:

    


    
    
};
