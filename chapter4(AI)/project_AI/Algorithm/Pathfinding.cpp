#include "PreCompiled.hpp"

struct GraphNode {
	// 각 노드는 인접 노드의 포인터들을 가지고 있따.
	std::vector<GraphNode*> mAdjacent;
};

struct Graph {
	// 그래프는 노드들을 포함한다.
	std::vector<GraphNode*> mNodes;
};

struct WeightedEdge {
	// 이 edge에 연결된 노드
	struct WeightedGraphNode* mFrom;
	struct WeightedGraphNode* mTo;

	// 이 edge의 가중치
	float mWeight;
};

struct WeightedGraphNode {
	// 외부로 향하는 edge를 저장한다.
	std::vector<WeightedEdge*> mEdges;
};

struct WeightedGraph {
	// 가중치를 가진 edge를 가진 노드들
	std::vector<WeightedGraphNode*> mNodes;
};

// 맵 타일
using NodeToParentMap =
std::unordered_map<const GraphNode*, const GraphNode*>;

bool BFS(const Graph& graph, const GraphNode* start, const GraphNode* goal,
	NodeToParentMap& outMap) {
	// 경로를 찾았는지를 알 수 있는 플래그
	bool pathFound = false;

	// 고려해야 된 노드
	std::queue<const GraphNode*> q;
	// 시작 노드를 큐에 넣는다
	q.emplace(start);

	while (!q.empty()) {
		// 큐에서 노드를 꺼낸다
		const GraphNode* current = q.front(); // 큐의 element에 접근
		q.pop();	// 큐에서 element삭제

		// 목표에 도달했는지 확인
		if (current == goal) {
			pathFound = true;
			break;
		}

		// 큐에는 없는 인접 노드를 꺼낸다
		for (const GraphNode* node : current->mAdjacent) {
			// 시작 노드를 제외하고
			// 부모가 null이면 큐에 넣지 않은 노드다
			const GraphNode* parent = outMap[node];
			if (parent == nullptr && node != start) {
				// 이 노드의 부모 노드를 설정하고 큐에 넣는다
				outMap[node] = current;
				q.emplace(node);
			}
		}
	}

	return pathFound;

}