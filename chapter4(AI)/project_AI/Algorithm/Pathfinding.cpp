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

// �� Ÿ��
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