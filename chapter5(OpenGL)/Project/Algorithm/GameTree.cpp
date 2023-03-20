#include "..\PreCompiled.hpp"

struct GameState {
	// ƽ���� ���� �� ĭ�� ����
	enum SquareState { Empty, X, O };
	// ƽ���� ���� ����
	SquareState mBoard[3][3];
};

// ���� Ʈ�� ���
struct GTNode {
	// �ڽ� ���
	std::vector<GTNode*> mChildren;
	// �� ����� ���� ����
	GameState mState;
};

// ���� ���� ����
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
	// ���� ���� ���� ���� �ִ��� Ȯ��
	for (int i = 0; i < 3; i++) {
		bool same = true;
		GameState::SquareState v = state.mBoard[i][0];
		for (int j = 1; j < 3; j++) {
			if (state.mBoard[i][j] != v) {
				same = false;
			}
		}

		if (same) {
			// ���� �̰���� Ȯ��
			if (v == GameState::X) {
				return 1.f;
			}
			else {
				return -1.f;
			}
		}
	}

	// ���� ���� ���� ���� �ִ��� Ȯ��
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

	// �밢�� �� ���� ���� �ִ��� Ȯ��
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

	// ���
	return 0.0f;
}
float MinPlayer(const GTNode* node);

// ���� �ش�ȭ�Ϸ��� �÷��̾�
float MaxPlayer(const GTNode* node) {
	// ���� ����� ������ �����Ѵ�.
	if (node->mChildren.empty()) {
		return GetScore(node->mState);
	}

	// �ִ밪�� ������ ���� Ʈ���� ã�´�.
	float maxValue = -std::numeric_limits<float>::infinity();
	for (const GTNode* child : node->mChildren) {
		maxValue = std::max(maxValue, MinPlayer(child));
	}

	return maxValue;
}

// ���� �ּ�ȭ�Ϸ��� �÷��̾�
float MinPlayer(const GTNode* node) {
	// ���� ����� ������ ��ȯ�Ѵ�.
	if (node->mChildren.empty()) {
		return GetScore(node->mState);
	}

	// �ּڰ��� ���� ���� Ʈ���� ã�´�.
	float minValue = std::numeric_limits<float>::infinity();
	for (const GTNode * child : node->mChildren) {
		minValue = std::min(minValue, MaxPlayer(child));
	}
	
	return minValue;
}

// �ּ��� �̵��� �����ϴ� �Լ�
const GTNode* MinimaxDecide(const GTNode* root) {
	// �ִ��� ���� ���� Ʈ���� ã�� �ش� ������ �����Ѵ�.
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
// �ҿ��� ���� Ʈ�� �ٷ��
// MinPlayer�� �ٸ� �Լ��鵵 �����ϰ� ������ �ʿ䰡 �ֵ�.
// GameState�� 3���� ��� �Լ� IsTerminal, GetScore, GetPossibleMoves ����
// IsTerminal : ���°� ���� �����̸� true��ȯ
// GetScore : ������� ���� ������ �޸���ƽ ���� return�ϰų� ���� ������ ������ ��ȯ
// GetPossibleMoves : ���� ���� ���Ŀ� ������ �� �ִ� �̵��� ���� ���� ���� ���͸� ����
float MaxPlayerLimit(const GameState* state, int depth) {
	// depth�� 0�̰ų� ���°� terminal�̸� �ִ� ���̿� �����ߴٴ� �� ���Ѵ�.
	if (depth == 0 || state->IsTerminal()) {
		return state->GetScore();
	}

	// �ִ밪�� ���� ���� Ʈ���� ã�´�
	float maxValue = -std::numeric_limits<float>::infinity();

	for (const GameState* child : state->GetPossibleMoves()) {
		maxValue = std::max(maxValue, MinPlayer(child, depth - 1));
	}

	return maxValue;
}


// �ҿ��� Ʈ���� ����
const GameState* AlphBetaDecide(const GameState* root, int maxDepth) {
	const GameState* choice = nullptr;
	// ���Ĵ� ���� ���Ѵ�, ��Ÿ�� ���� ���Ѵ� ������ ����
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
			return maxValue; // ��Ÿ ����ġ��
		}

		alpha = std::max(maxValue, alph) // ���İ��� ����
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
			return minValue; // ���� ����ġ��
		}
		beta = std::min(minValue, beta); // ��Ÿ���� ���ҽ�Ų��.
	}
	
	return minValue;
}
*/
float AlphaBetaMin(const GTNode* node, float alpha, float beta); 

// ���� ���� Ʈ���� ����� ���� ��Ÿ ����ġ��
float AlphaBetaMax(const GTNode* node, float alpha, float beta) {
	// ���� ���� ���ھ� return
	if (node->mChildren.empty()) {
		return GetScore(node->mState);
	}

	float maxValue = -std::numeric_limits<float>::infinity();

	// ���� Ʈ���� ���� ū ���� ã�´�
	for (const GTNode* child : node->mChildren) {
		maxValue = std::max(maxValue, AlphaBetaMin(child, alpha, beta));
		if (maxValue >= beta) {
			return maxValue; // ��Ÿ ����ġ��
		}
		alpha = std::max(maxValue, alpha);
	}

	return maxValue;
}

float AlphaBetaMin(const GTNode* node, float alpha, float beta) {
	// ���� ��常 �ִٸ� ���� ��ȯ
	if (node->mChildren.empty()) {
		return GetScore(node->mState);
	}

	float minValue = std::numeric_limits<float>::infinity();

	// ���� Ʈ���� ���� ���� �� ã��
	for (const GTNode* child : node->mChildren) {
		minValue = std::min(minValue, AlphaBetaMax(child, alpha, beta));
		if (minValue <= alpha) {
			return minValue; // ���� ����ġ��
		}
		beta = std::min(minValue, beta);
	}

	return minValue;
}

const GTNode* AlphaBetaDecide(const GTNode* root) {
	const GTNode* choice = nullptr;
	// ���Ĵ� ���� ���Ѵ�, ��Ÿ�� ���� ���Ѵ� ������ ����
	float maxValue = -std::numeric_limits<float>::infinity();
	float beta = std::numeric_limits<float>::infinity();

	// ���� Ʈ���� ���� ū ���� ���� Ʈ�� ã��
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
