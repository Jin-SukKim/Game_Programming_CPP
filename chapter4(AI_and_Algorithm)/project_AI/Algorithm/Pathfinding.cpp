#include "PreCompiled.hpp"

struct GraphNode {
	// �� ���� ���� ����� �����͵��� ������ �ֵ�.
	std::vector<GraphNode*> mAdjacent;
};

struct Graph {
	// �׷����� ������ �����Ѵ�.
	std::vector<GraphNode*> mNodes;
};

struct WeightedEdge {
	// �� edge�� ����� ���
	struct WeightedGraphNode* mFrom;
	struct WeightedGraphNode* mTo;

	// �� edge�� ����ġ
	float mWeight;
};

struct WeightedGraphNode {
	// �ܺη� ���ϴ� edge�� �����Ѵ�.
	std::vector<WeightedEdge*> mEdges;
};

struct WeightedGraph {
	// ����ġ�� ���� edge�� ���� ����
	std::vector<WeightedGraphNode*> mNodes;
};

// �� Ÿ��, �θ� ��� �Ǻ��� ���� ��
using NodeToParentMap =
std::unordered_map<const GraphNode*, const GraphNode*>;

bool BFS(const Graph& graph, const GraphNode* start, const GraphNode* goal,
	NodeToParentMap& outMap) {
	// ��θ� ã�Ҵ����� �� �� �ִ� �÷���
	bool pathFound = false;

	// ����ؾ� �� ���
	std::queue<const GraphNode*> q;
	// ���� ��带 ť�� �ִ´�
	q.emplace(start);

	while (!q.empty()) {
		// ť���� ��带 ������
		const GraphNode* current = q.front(); // ť�� element�� ����
		q.pop();	// ť���� element����

		// ��ǥ�� �����ߴ��� Ȯ��
		if (current == goal) {
			pathFound = true;
			break;
		}

		// ť���� ���� ���� ��带 ������
		for (const GraphNode* node : current->mAdjacent) {
			// ���� ��带 �����ϰ�
			// �θ� null�̸� ť�� ���� ���� ����
			const GraphNode* parent = outMap[node];
			if (parent == nullptr && node != start) {
				// �� ����� �θ� ��带 �����ϰ� ť�� �ִ´�
				outMap[node] = current;
				q.emplace(node);
			}
		}
	}

	return pathFound;

}

// GBFS Ž���� ���� �߰� �����͵�
struct GBFSScratch {
	const WeightedEdge* mParentEdge = nullptr;
	float mHeuristic = 0.f;
	bool mInOpenSet = false;
	bool mInClosedSet = false;
};

// �� ��忡 ���迡�� ����� ��ġ ������ �ʿ��ϴ�.
float ComputeHeuristic(const WeightedGraphNode* node, const WeightedGraphNode* goal) {
	return 0.f;
}
// Ű�� ����� �������̰� ���� GBFS�� �߰� �����͵��� ��
using GBFSMap =
std::unordered_map<const WeightedGraphNode*, GBFSScratch>;

// ���� ���� �������� ��� ��带 ���캻��
// ���� ���տ� ���� ���� ��常�� ����Ѵ�
// �� ������ ���� ���κ��� �ڽ��� ���� ���ϴ� �θ� edge�� ������.
// ���� ���տ� ���� ����� �޸���ƽ�� ����ϰ� ���� ���տ� �߰��Ѵ�.
bool GBFS(const WeightedGraph& g, const WeightedGraphNode* start,
	const WeightedGraphNode* goal, GBFSMap& outMap) {

	std::vector<const WeightedGraphNode*> openSet;
	
	// ���� ��带 ���� ���� �����ϰ�
	const WeightedGraphNode* current = start;
	// ���� ���տ� �ִٰ� ��ŷ�Ѵ�.
	outMap[current].mInClosedSet = true;

	do {
		// ���� ��带 ���� ���տ� �߰��Ѵ�.
		for (const WeightedEdge* edge : current->mEdges) {
			// ���� ����� �߰� �����͸� ��´�
			GBFSScratch& data = outMap[edge->mTo];
			// �� ��尡 ���� ���տ� ���ٸ� �߰��Ѵ�.
			if (!data.mInClosedSet) {
				// ���� ����� �θ� ������ ����
				data.mParentEdge = edge;
				if (!data.mInOpenSet) {
					// �� ����� �޸���ƽ�� ����ϰ� ���� ���տ� �߰��Ѵ�.
					data.mHeuristic = ComputeHeuristic(edge->mTo, goal);
					data.mInOpenSet = true;
					openSet.emplace_back(edge->mTo);
				}
			}
		}

		// ���� ��忡 ������ ��带 ó���� �� ���� ������ ���캻��.

		// ���� ������ ����ִٸ� ���� ��尡 ���ٴ� ���̴�.
		// �� ���������� ����ǥ�������� ��ΰ� ���� �� �߻��Ѵ�.
		if (openSet.empty()) {
			break;
		}

		// ���� ���տ� ��尡 �ִٸ� �˰����� ��� �����Ѵ�.
		// ���� ���տ� �ð��� ���� �޸���ƽ ����� ���� ��带 ã�� ���� �������� �̵��Ѵ�.
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			// lambda�Լ��� �޸���ƽ ����� �����ش�.
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
				return outMap[a].mHeuristic < outMap[b].mHeuristic;
			});

		// ���� ��带 �����ϰ� �� ��带 ���� ���տ� �ô��� �������� �̵��Ѵ�.
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInClosedSet = true;
	} while (current != goal); // ���� ��尡 ��ǥ ��尡 �ƴ϶�� ��� ����

	// ���� ���� ��ǥ ����� ���� ���� GBFS��
	// ��θ� �߰��ߴ��� ���θ� �Ǵ��Ѵ�.
	return (current == goal) ? true : false;

}

// A* Ž���� ���� �߰� �����͵�
struct AStarScratch {
	const WeightedEdge* mParentEdge = nullptr;
	float mHeuristic = 0.f;
	float mActualFromStart = 0.f;
	bool mInOpenSet = false;
	bool mInClosedSet = false;
};

// �θ� ��� �ľ��� ���� ��
using AStarMap =
std::unordered_map<const WeightedGraphNode*, AStarScratch>;

bool AStar(const WeightedGraph& g, const WeightedGraphNode* start,
	const WeightedGraphNode* goal, AStarMap& outMap) {

	std::vector<const WeightedGraphNode*> openSet;

	// ���� ��� �����ϰ� ���� ���տ� ����
	const WeightedGraphNode* current = start;
	outMap[current].mInClosedSet = true;

	do {
		// ���� ��带 ���� ���տ� �ִ´�.
		for (const WeightedEdge* edge : current->mEdges) {
			// ���� ���
			const WeightedGraphNode* neighbor = edge->mTo;
			// ����� �߰� ������
			AStarScratch& data = outMap[neighbor];
			// ���� ���տ� ������ Ȯ��
			if (!data.mInClosedSet) {
				// ���� ���տ� ������ Ȯ��
				if (!data.mInOpenSet) {
					// ���� ���տ� ���ٸ� �θ�� �ݵ��
					// ���� ���տ� �־�� �Ѵ�.
					data.mParentEdge = edge;
					// �޸���ƽ ���
					data.mHeuristic = ComputeHeuristic(neighbor, goal);
					// ���� ����� �θ��� ���� ��� + �θ𿡼� �ڽ����� �̵��ϴ� edge�� ����ġ
					data.mActualFromStart = outMap[current].mActualFromStart
						+ edge->mWeight;
					data.mInOpenSet = true;
					openSet.emplace_back(neighbor);
				
				}
				// ���� ���տ� �ִٸ�
				else {
					// ���� ��尡 �θ� ��尡 ������ �Ǵ��ϰ���
					// ���ο� ���� ����� ����Ѵ�.
					float newG = outMap[current].mActualFromStart + edge->mWeight;
					if (newG < data.mActualFromStart) {
						// ���� ��尡 �� ����� �θ� ���� ä�õ�
						data.mParentEdge = edge;
						data.mActualFromStart = newG;
					}
				}
			}
		}

		// ���� ������ ����ٸ� ������ ��δ� ���ٴ� ��
		if (openSet.empty()) {
			break;
		}

		// ���� ���տ� ���� ���� ����� ��� ��带 ã�´�
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
				// f(x) �� ���
				float fOfA = outMap[a].mHeuristic + outMap[a].mActualFromStart;
				float fOfB = outMap[b].mHeuristic + outMap[b].mActualFromStart;
				return fOfA < fOfB;
			});

		// ���� ��带 �����ϰ� ���� ���տ��� ���� �������� �Ű��ش�
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInClosedSet = true;
	} while (current != goal);

	return (current == goal) ? true : false;
} 