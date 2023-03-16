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

// 맵 타일, 부모 노드 판별을 위한 맵
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

// GBFS 탐색을 위한 추가 데이터들
struct GBFSScratch {
	const WeightedEdge* mParentEdge = nullptr;
	float mHeuristic = 0.f;
	bool mInOpenSet = false;
	bool mInClosedSet = false;
};

// 각 노드에 세계에서 노드의 위치 정보가 필요하다.
float ComputeHeuristic(const WeightedGraphNode* node, const WeightedGraphNode* goal) {
	return 0.f;
}
// 키가 노드의 포인터이고 값이 GBFS의 추가 데이터들인 맵
using GBFSMap =
std::unordered_map<const WeightedGraphNode*, GBFSScratch>;

// 현재 노드오 ㅏ인접한 모드 노드를 살펴본다
// 닫힌 집합에 있지 않은 노드만을 고려한다
// 이 노드들은 현재 노드로부터 자신의 노드로 향하는 부모 edge를 가진다.
// 열린 집합에 없는 노드의 휴리스틱을 계산하고 열린 집합에 추가한다.
bool GBFS(const WeightedGraph& g, const WeightedGraphNode* start,
	const WeightedGraphNode* goal, GBFSMap& outMap) {

	std::vector<const WeightedGraphNode*> openSet;
	
	// 시작 노드를 현재 노드로 설정하고
	const WeightedGraphNode* current = start;
	// 닫힌 집합에 있다고 마킹한다.
	outMap[current].mInClosedSet = true;

	do {
		// 인접 노드를 열린 집합에 추가한다.
		for (const WeightedEdge* edge : current->mEdges) {
			// 인접 노드의 추가 데이터를 얻는다
			GBFSScratch& data = outMap[edge->mTo];
			// 이 노드가 닫힌 집합에 없다면 추가한다.
			if (!data.mInClosedSet) {
				// 인접 노드의 부모 에지를 설정
				data.mParentEdge = edge;
				if (!data.mInOpenSet) {
					// 이 노드의 휴리스틱을 계산하고 열린 집합에 추가한다.
					data.mHeuristic = ComputeHeuristic(edge->mTo, goal);
					data.mInOpenSet = true;
					openSet.emplace_back(edge->mTo);
				}
			}
		}

		// 현재 노드에 인접한 노드를 처리한 후 열린 집합을 살펴본다.

		// 열린 집합이 비어있다면 평가할 노드가 없다는 뜻이다.
		// 즉 시작점에서 ㅁ고표점까지의 경로가 없을 때 발생한다.
		if (openSet.empty()) {
			break;
		}

		// 열린 집합에 노드가 있다면 알고리즘을 계속 수행한다.
		// 열린 집합엣 ㅓ가장 낮은 휴리스틱 비용을 가진 노드를 찾아 닫힌 집합으로 이동한다.
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			// lambda함수로 휴리스틱 비용을 비교해준다.
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
				return outMap[a].mHeuristic < outMap[b].mHeuristic;
			});

		// 현재 노드를 설정하고 이 노드를 열린 집합엣 ㅓ닫힌 집합으로 이동한다.
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInClosedSet = true;
	} while (current != goal); // 현재 노드가 목표 노드가 아니라면 계속 진행

	// 현재 노드와 목표 노드의 값을 비교해 GBFS가
	// 경로를 발견했는지 여부를 판단한다.
	return (current == goal) ? true : false;

}

// A* 탐색을 위한 추가 데이터들
struct AStarScratch {
	const WeightedEdge* mParentEdge = nullptr;
	float mHeuristic = 0.f;
	float mActualFromStart = 0.f;
	bool mInOpenSet = false;
	bool mInClosedSet = false;
};

// 부모 노드 파악을 위한 맵
using AStarMap =
std::unordered_map<const WeightedGraphNode*, AStarScratch>;

bool AStar(const WeightedGraph& g, const WeightedGraphNode* start,
	const WeightedGraphNode* goal, AStarMap& outMap) {

	std::vector<const WeightedGraphNode*> openSet;

	// 시작 노드 설정하고 닫힌 집합에 설정
	const WeightedGraphNode* current = start;
	outMap[current].mInClosedSet = true;

	do {
		// 인접 노드를 열린 집합에 넣는다.
		for (const WeightedEdge* edge : current->mEdges) {
			// 인접 노드
			const WeightedGraphNode* neighbor = edge->mTo;
			// 노드의 추가 데이터
			AStarScratch& data = outMap[neighbor];
			// 닫힌 집합에 없는지 확인
			if (!data.mInClosedSet) {
				// 열린 집합에 없는지 확인
				if (!data.mInOpenSet) {
					// 열린 집합에 없다면 부모는 반드시
					// 열린 집합에 있어야 한다.
					data.mParentEdge = edge;
					// 휴리스틱 비용
					data.mHeuristic = ComputeHeuristic(neighbor, goal);
					// 실제 비용은 부모의 실제 비용 + 부모에서 자신으로 이동하는 edge의 가중치
					data.mActualFromStart = outMap[current].mActualFromStart
						+ edge->mWeight;
					data.mInOpenSet = true;
					openSet.emplace_back(neighbor);
				
				}
				// 열린 집합에 있다면
				else {
					// 현재 노드가 부모 노드가 될지를 판단하고자
					// 새로운 실제 비용을 계산한다.
					float newG = outMap[current].mActualFromStart + edge->mWeight;
					if (newG < data.mActualFromStart) {
						// 현재 노드가 이 노드의 부모 노드로 채택됨
						data.mParentEdge = edge;
						data.mActualFromStart = newG;
					}
				}
			}
		}

		// 열린 집합이 비었다면 가능한 경로는 없다는 뜻
		if (openSet.empty()) {
			break;
		}

		// 열린 집합엣 가장 적은 비용이 드는 노드를 찾는다
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
				// f(x) 값 계산
				float fOfA = outMap[a].mHeuristic + outMap[a].mActualFromStart;
				float fOfB = outMap[b].mHeuristic + outMap[b].mActualFromStart;
				return fOfA < fOfB;
			});

		// 현재 노드를 성정하고 열린 집합에서 닫힌 집합으로 옮겨준다
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInClosedSet = true;
	} while (current != goal);

	return (current == goal) ? true : false;
} 