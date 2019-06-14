#ifndef _BVH_ACCEL_
#define _BVH_ACCEL_

#include "Primitive.h"
#include <vector>
#include <memory>
#include <algorithm>

namespace target{

	enum class SplitMethod { Middle, EqualCounts };

	struct BVHPrimitiveInfo {
		BVHPrimitiveInfo(size_t primitiveNumber, Bounds3 bounds)
			: primitiveNumber(primitiveNumber), bounds(bounds),
			centroid(Vec3(bounds.pMin * .5 + bounds.pMax * .5)) { }
		BVHPrimitiveInfo(){}

		size_t primitiveNumber;
		Bounds3 bounds;
		Vec3 centroid;
	};

	struct BVHBuildNode {
		Bounds3 bounds;
		BVHBuildNode * children[2];
		int splitAxis, firstPrimOffset, nPrimitives;

		void InitLeaf(int first, int n, const Bounds3 &b) {
			firstPrimOffset = first;
			nPrimitives = n;
			bounds = b;
			children[0] = children[1] = nullptr;
		}

		void InitInterior(int axis, BVHBuildNode *c0, BVHBuildNode *c1) {
			children[0] = c0;
			children[1] = c1;
			bounds = Union(c0->bounds, c1->bounds);
			splitAxis = axis;
			nPrimitives = 0;
		}
	};

	struct LinearBVHNode {
		Bounds3 bounds;
		union {
			int primitivesOffset; // leaf
			int secondChildOffset; // interior
		};
		uint16_t nPrimitives; // 0 -> interior node
		uint8_t axis; // interior node: xyz
		uint8_t pad[1]; // ensure 32 byte total size
	};

	class BVHAccel : public Primitive{
		private: // Private attributes
			int maxPrimsInNode;
			int offset;
			SplitMethod splitMethod;
			BVHBuildNode *root = nullptr;
			LinearBVHNode *nodes = nullptr;
		
		public:
			std::vector<std::shared_ptr<Primitive>> primitives;
			BVHAccel(const std::vector<std::shared_ptr<Primitive>> & primitives, int maxPrimsInNode, 
				SplitMethod splitMethod ) : 
				maxPrimsInNode(std::min(255, maxPrimsInNode)), 
			 	primitives(primitives), splitMethod(splitMethod){
					if(primitives.empty()) return;
			}

			inline void BVHBuild(){
				std::vector< BVHPrimitiveInfo > primitiveInfo( primitives.size() );
				for (size_t i = 0; i < primitives.size(); ++i)
					primitiveInfo[i] = BVHPrimitiveInfo(i, primitives[i]->get_bounding_box());

				int totalNodes = 0;
				std::vector<std::shared_ptr<Primitive>> orderedPrims;

				root = recursiveBuild(primitiveInfo, 0, primitives.size(), &totalNodes, orderedPrims);				
				primitives.swap(orderedPrims);

				//std::cout << "Total nodes: " << totalNodes << std::endl;
				//print_tree(root);

				offset = 0;
				nodes = new LinearBVHNode[totalNodes];
				flattenBVHTree(root, &offset);

				//print_flatten_tree(nodes, totalNodes);
			}

			bool intersect( Ray& r, SurfaceInteraction * isect) const{
				bool hit = false;
				Vec3 invDir = 1.0 / r.getDirection();
				int dirIsNeg[3] = { invDir.x() < 0, invDir.y() < 0, invDir.z() < 0 };

				int toVisitOffset = 0, currentNodeIndex = 0;
				int nodesToVisit[64];

				while (true) {
					const LinearBVHNode *node = &nodes[currentNodeIndex];
					// Check ray against BVH node
					if (node->bounds.intersect_p(r, invDir, dirIsNeg)) {
						if (node->nPrimitives > 0) {
							// Intersect ray with primitives in leaf BVH node
							for (int i = 0; i < node->nPrimitives; ++i)
								if (primitives[node->primitivesOffset + i]->intersect(r, isect)){
									hit = true;
								}
							if (toVisitOffset == 0) break;
							currentNodeIndex = nodesToVisit[--toVisitOffset];
						}else{
							// Put far bvh node on nodesToVisit stack, advance to near node
							if (dirIsNeg[node->axis]) {
								nodesToVisit[toVisitOffset++] = currentNodeIndex + 1;
								currentNodeIndex = node->secondChildOffset;
							} else {
								nodesToVisit[toVisitOffset++] = node->secondChildOffset;
								currentNodeIndex = currentNodeIndex + 1;
							}
						}
					}else{
						if (toVisitOffset == 0) break;
						currentNodeIndex = nodesToVisit[--toVisitOffset];
					}
				}
				return hit;
			}

			bool intersect_p( const Ray& r ) const{
				Ray ray = r;
				return intersect(ray, nullptr);
			}
			bool intersect_p( const Ray& r, double tmin, double tmax ) const{ // TODO
				Ray ray = r;
				Vec3 invDir = 1.0 / ray.getDirection();
				int dirIsNeg[3] = { invDir.x() < 0, invDir.y() < 0, invDir.z() < 0 };

				int toVisitOffset = 0, currentNodeIndex = 0;
				int nodesToVisit[64];

				while (true) {
					const LinearBVHNode *node = &nodes[currentNodeIndex];
					// Check ray against BVH node
					if (node->bounds.intersect_p(ray, invDir, dirIsNeg)) {
						if (node->nPrimitives > 0) {
							// Intersect ray with primitives in leaf BVH node
							for (int i = 0; i < node->nPrimitives; ++i)
								if (primitives[node->primitivesOffset + i]->intersect_p(ray, tmin, tmax)){
									return true;
								}
							if (toVisitOffset == 0) break;
							currentNodeIndex = nodesToVisit[--toVisitOffset];
						}else{
							// Put far bvh node on nodesToVisit stack, advance to near node
							if (dirIsNeg[node->axis]) {
								nodesToVisit[toVisitOffset++] = currentNodeIndex + 1;
								currentNodeIndex = node->secondChildOffset;
							} else {
								nodesToVisit[toVisitOffset++] = node->secondChildOffset;
								currentNodeIndex = currentNodeIndex + 1;
							}
						}
					}else{
						if (toVisitOffset == 0) break;
						currentNodeIndex = nodesToVisit[--toVisitOffset];
					}
				}
				return false;
			}

		private: // Private methods
			inline BVHBuildNode * recursiveBuild(
				std::vector<BVHPrimitiveInfo> &primitiveInfo, int start,
				int end, int *totalNodes, std::vector<std::shared_ptr<Primitive>> & orderedPrims){

				BVHBuildNode * node = new BVHBuildNode();

				(*totalNodes)++;

				// Compute bounds of all primitives in BVH node
				Bounds3 bounds;
				for (int i = start; i < end; ++i)
					bounds = Union(bounds, primitiveInfo[i].bounds);

				int nPrimitives = end - start;
				if(nPrimitives == 1){
					// Create leaf BVHBuildNode
					int firstPrimOffset = orderedPrims.size();
					for (int i = start; i < end; ++i) {
						int primNum = primitiveInfo[i].primitiveNumber;
						orderedPrims.push_back(primitives[primNum]);
					}
					node->InitLeaf(firstPrimOffset, nPrimitives, bounds);
				}else{
					// Compute bound of primitive centroids, choose split dimension
					Bounds3 centroidBounds;
					for(int i = start; i < end; ++i)
						centroidBounds = Union(centroidBounds, primitiveInfo[i].centroid);
					int dim = centroidBounds.MaximumExtent();
					// Partition primitives into two sets and build children
					int mid = (start + end) / 2;
					if(centroidBounds.pMax[dim] == centroidBounds.pMin[dim]){
						// Create leaf BVHBuildNode
						int firstPrimOffset = orderedPrims.size();
						for (int i = start; i < end; ++i) {
							int primNum = primitiveInfo[i].primitiveNumber;
							orderedPrims.push_back(primitives[primNum]);
						}
						node->InitLeaf(firstPrimOffset, nPrimitives, bounds);
					}else{
						// Partition primitives on splitMethod
						if(splitMethod == SplitMethod::Middle){
							double pmid = (centroidBounds.pMin[dim] + centroidBounds.pMax[dim]) / 2;
							BVHPrimitiveInfo *midPtr = 
								std::partition(&primitiveInfo[start], &primitiveInfo[end-1]+1,
									[dim, pmid](const BVHPrimitiveInfo &pi) {
										return pi.centroid[dim] < pmid;
									});
							mid = midPtr - &primitiveInfo[0];
							if(!(mid != start && mid != end)){
								mid = (start + end) / 2;
								std::nth_element(&primitiveInfo[start], &primitiveInfo[mid], &primitiveInfo[end-1]+1,
									[dim](const BVHPrimitiveInfo &a, const BVHPrimitiveInfo &b) {
										return a.centroid[dim] < b.centroid[dim];
									});
							}								
						}else{
							mid = (start + end) / 2;
							std::nth_element(&primitiveInfo[start], &primitiveInfo[mid], &primitiveInfo[end-1]+1,
								[dim](const BVHPrimitiveInfo &a, const BVHPrimitiveInfo &b) {
									return a.centroid[dim] < b.centroid[dim];
								});
						}

						node->InitInterior(dim, 
							recursiveBuild(primitiveInfo, start, mid, totalNodes, orderedPrims),
							recursiveBuild(primitiveInfo, mid, end, totalNodes, orderedPrims));
					}

				}
				return node;
			}

			inline int flattenBVHTree(BVHBuildNode *node, int *offset){
				LinearBVHNode *linearNode = &nodes[*offset];
				linearNode->bounds = node->bounds;
				int myOffset = (*offset)++;

				if (node->nPrimitives > 0) {
					linearNode->primitivesOffset = node->firstPrimOffset;
					linearNode->nPrimitives = node->nPrimitives;
				}else{
					linearNode->axis = node->splitAxis;
					linearNode->nPrimitives = 0;
					flattenBVHTree(node->children[0], offset);
					linearNode->secondChildOffset =	flattenBVHTree(node->children[1], offset);
				}

				return myOffset;
			}

			void print_tree(BVHBuildNode* root) const {
			    if (root->children[0] == nullptr && root->children[1] == nullptr)
			        std::cout << "l: " << root->firstPrimOffset << "/" << root->nPrimitives << std::endl;         
			    std::cout << "i: " << root->firstPrimOffset << "/" << root->nPrimitives << std::endl;                                                                             
			    if (root->children[0] != nullptr) { 
			        print_tree(root->children[0]);                                                                                                                             
			    } 
			    if (root->children[1] != nullptr) { 
			        print_tree(root->children[1]);                                                                                                                             
			    }   
			}

			void print_flatten_tree(LinearBVHNode *nodes, const int & totalNodes) const {
				for (int i = 0; i < totalNodes; ++i){
					std::cout << "i: " << nodes[i].primitivesOffset << "/" << nodes[i].nPrimitives << std::endl;
				}  
			}
	};

}

#endif