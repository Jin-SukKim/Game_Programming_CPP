#include "..\PreCompiled.hpp"

struct GameState {
	// 틱택토 게임 각 칸의 상태
	enum SquareState { Empty, X, O };
	// 틱택토 게임 보드
	SquareState mBoard[3][3];
};

// 게임 트리 노드
struct GTNode {
	// 자식 노드
	std::vector<GTNode*> mChildren;
	// 이 노드의 게임 상태
	GameState mState;
};

// 게임 상태 갱신
void GenStates(GTNode* root, bool xPlayer) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (root->mState.mBoard[i][j] == GameState::Empty) {
				GTNode* node = new GTNode;
				root->mChildren.emplace_back(node);
				node->mState = root->mState;
				node->mState.mBoard[i][j] = xPlayer ? GameState::X : GameState::O;
				GenStates(node, !xPlayer);
			}
		}
	}
}

float GetScore(const GameState& state) {
	// 열이 전부 같은 줄이 있는지 확인
	for (int i = 0; i < 3; i++) {
		bool same = true;
		GameState::SquareState v = state.mBoard[i][0];
		for (int j = 1; j < 3; j++) {
			if (state.mBoard[i][j] != v) {
				same = false;
			}
		}

		if (same) {
			// 누가 이겼는지 확인
			if (v == GameState::X) {
				return 1.f;
			}
			else {
				return -1.f;
			}
		}
	}

	// 행이 전부 같은 줄이 있는지 확인
	for (int j = 0; j < 3; j++)
	{
		bool same = true;
		GameState::SquareState v = state.mBoard[0][j];
		for (int i = 1; i < 3; i++)
		{
			if (state.mBoard[i][j] != v)
			{
				same = false;
			}
		}

		if (same)
		{
			if (v == GameState::X)
			{
				return 1.0f;
			}
			else
			{
				return -1.0f;
			}
		}
	}

	// 대각선 중 같은 줄이 있는지 확인
	if (((state.mBoard[0][0] == state.mBoard[1][1]) &&
		(state.mBoard[1][1] == state.mBoard[2][2])) ||
		((state.mBoard[2][0] == state.mBoard[1][1]) &&
			(state.mBoard[1][1] == state.mBoard[0][2])))
	{
		if (state.mBoard[1][1] == GameState::X)
		{
			return 1.0f;
		}
		else
		{
			return -1.0f;
		}
	}

	// 비김
	return 0.0f;
}
float MinPlayer(const GTNode* node);

// 점수 극대화하려는 플레이어
float MaxPlayer(const GTNode* node) {
	// 리프 노드라면 점수를 리턴한다.
	if (node->mChildren.empty()) {
		return GetScore(node->mState);
	}

	// 최대값을 가지는 하위 트리를 찾는다.
	float maxValue = -std::numeric_limits<float>::infinity();
	for (const GTNode* child : node->mChildren) {
		maxValue = std::max(maxValue, MinPlayer(child));
	}

	return maxValue;
}

// 점수 최소화하려는 플레이어
float MinPlayer(const GTNode* node) {
	// 리프 노드라면 점수를 반환한다.
	if (node->mChildren.empty()) {
		return GetScore(node->mState);
	}

	// 최솟값을 가진 하위 트리를 찾는다.
	float minValue = std::numeric_limits<float>::infinity();
	for (const GTNode * child : node->mChildren) {
		minValue = std::min(minValue, MaxPlayer(child));
	}
	
	return minValue;
}

// 최선의 이동을 결정하는 함수
const GTNode* MinimaxDecide(const GTNode* root) {
	// 최댓값을 가진 하위 트리를 찾고 해당 선택을 저장한다.
	const GTNode* choice = nullptr;
	float maxValue = -std::numeric_limits<float>::infinity();
	for (const GTNode* child : root->mChildren) {
		float v = MinPlayer(child);
		
		if (v > maxValue) {
			maxValue = v;
			choice = child;
		}
	}

	return choice;
}

/*
// 불완전 게임 트리 다루기
// MinPlayer등 다른 함수들도 유사하게 수정할 필요가 있따.
// GameState가 3개의 멤버 함수 IsTerminal, GetScore, GetPossibleMoves 가정
// IsTerminal : 상태가 종료 상태이면 true반환
// GetScore : 종료되지 않은 상태의 휴리스틱 값을 return하거나 종료 상태의 점수를 반환
// GetPossibleMoves : 현재 상태 이후에 움직일 수 있는 이동에 대한 게임 상태 벡터를 리턴
float MaxPlayerLimit(const GameState* state, int depth) {
	// depth가 0이거나 상태가 terminal이면 최대 깊이에 도달했다는 걸 뜻한다.
	if (depth == 0 || state->IsTerminal()) {
		return state->GetScore();
	}

	// 최대값을 가진 하위 트리를 찾는다
	float maxValue = -std::numeric_limits<float>::infinity();

	for (const GameState* child : state->GetPossibleMoves()) {
		maxValue = std::max(maxValue, MinPlayer(child, depth - 1));
	}

	return maxValue;
}


// 불완전 트리용 루프
const GameState* AlphBetaDecide(const GameState* root, int maxDepth) {
	const GameState* choice = nullptr;
	// 알파는 음의 무한대, 베타는 양의 무한대 값으로 시작
	float maxValue = -std::numeric_limits<float>::infinity();
	float minValue = std::numeric_limits<float>::infinity();

	
	for (const GameState* child : root->GetPossibleMoves()) {
		float v = AlphBetaMin(child, maxDepth - 1, maxValue, beta);
		if (v > maxValue) {
			maxValue = v;
			choice = child;
		}
	}
	return choice;
}


float AlphaBetaMax(const GameState* node, int depth, float alph, float beta) {
	if (depth == 0 || node->IsTerminal()) {
		return node->GetScore();
	}

	float maxValue = -std::numeric_limits<float>::infinity();
	for (const GameState* child : node->GetPossibleMoves()) {
		maxValue = std::max(maxValue, AlphaBetaMin(child, depth - 1, alpha, beta));

		if (maxValue >= beta) {
			return maxValue; // 베타 가지치기
		}

		alpha = std::max(maxValue, alph) // 알파값을 증가
	}

}

float AlphaBetaMin(const GameState* node, int depth, float alpha, float beta) {
	if (depth == 0 || node->IsTerminal()) {
		return node->GetScore();
	}

	float minValue = std::numeric_limits<float>::infinity();
	for (const GameState* child : node->GetPossibleMoves()) {
		minValue = std::min(minValue, AlphaBetaMax(child, depth - 1, alpha, beta));

		if (minValue <= alpha) {
			return minValue; // 알파 가지치기
		}
		beta = std::min(minValue, beta); // 베타값을 감소시킨다.
	}
	
	return minValue;
}
*/
float AlphaBetaMin(const GTNode* node, float alpha, float beta); 

// 완전 게임 트리를 사용한 알파 베타 가지치기
float AlphaBetaMax(const GTNode* node, float alpha, float beta) {
	// 리프 노드면 스코어 return
	if (node->mChildren.empty()) {
		return GetScore(node->mState);
	}

	float maxValue = -std::numeric_limits<float>::infinity();

	// 하위 트리의 가장 큰 값을 찾는다
	for (const GTNode* child : node->mChildren) {
		maxValue = std::max(maxValue, AlphaBetaMin(child, alpha, beta));
		if (maxValue >= beta) {
			return maxValue; // 베타 가지치기
		}
		alpha = std::max(maxValue, alpha);
	}

	return maxValue;
}

float AlphaBetaMin(const GTNode* node, float alpha, float beta) {
	// 리프 노드만 있다면 점수 반환
	if (node->mChildren.empty()) {
		return GetScore(node->mState);
	}

	float minValue = std::numeric_limits<float>::infinity();

	// 하위 트리중 가장 작은 수 찾기
	for (const GTNode* child : node->mChildren) {
		minValue = std::min(minValue, AlphaBetaMax(child, alpha, beta));
		if (minValue <= alpha) {
			return minValue; // 알파 가지치기
		}
		beta = std::min(minValue, beta);
	}

	return minValue;
}

const GTNode* AlphaBetaDecide(const GTNode* root) {
	const GTNode* choice = nullptr;
	// 알파는 음의 무한대, 베타는 양의 무한대 값으로 시작
	float maxValue = -std::numeric_limits<float>::infinity();
	float beta = std::numeric_limits<float>::infinity();

	// 하위 트리중 가장 큰 값을 가진 트리 찾기
	for (const GTNode* child : root->mChildren) {
		float v = AlphaBetaMin(child, maxValue, beta);
		if (v > maxValue) {
			maxValue = v;
			choice = child;
		}
	}
	return choice;
}

void testTicTac()
{
	GTNode* root = new GTNode;
	root->mState.mBoard[0][0] = GameState::O;
	root->mState.mBoard[0][1] = GameState::Empty;
	root->mState.mBoard[0][2] = GameState::X;
	root->mState.mBoard[1][0] = GameState::X;
	root->mState.mBoard[1][1] = GameState::O;
	root->mState.mBoard[1][2] = GameState::O;
	root->mState.mBoard[2][0] = GameState::X;
	root->mState.mBoard[2][1] = GameState::Empty;
	root->mState.mBoard[2][2] = GameState::Empty;

	GenStates(root, true);
	const GTNode* choice = AlphaBetaDecide(root);
	std::cout << choice->mChildren.size();
}
