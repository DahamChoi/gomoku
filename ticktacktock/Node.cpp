// Fill out your copyright notice in the Description page of Project Settings.


#include "Node.h"
#include "Math/UnrealMathUtility.h"

Node::Node(State* state)
{
	this->state = state;
	this->w = 0;
	this->n = 0;
}

Node::~Node()
{
	delete this->state;
	for (int i = 0; i < this->child_nodes.Num(); i++) {
		delete this->child_nodes[i];
	}
}

void Node::expand(void)
{
	TArray<int32> * smart_legal_actions = this->state->smartLegalActions();
	for (int i = 0; i < smart_legal_actions->Num(); i++) {
		this->child_nodes.Add(new Node(this->state->next((*smart_legal_actions)[i])));
	}
}

float Node::evaluate(void)
{
	float value = 0;

	if (this->state->isDone()) {
		value = this->state->isLose() ? -1 : 0;

		this->w += value;
		this->n += 1;
		return value;
	}

	if (this->child_nodes.Num() == 0) {
		value = playout(new State(this->state->mPieces,this->state->mEnemyPieces));

		this->w += value;
		this->n += 1;

		if (this->n == 10) {
			this->expand();
		}

		return value;
	}
	else {
		value = -this->next_child_node()->evaluate();

		this->w += value;
		this->n += 1;

		return value;
	}
}

Node* Node::next_child_node(void)
{
	for (int i = 0; i < this->child_nodes.Num(); i++) {
		if (this->child_nodes[i]->n == 0) {
			return this->child_nodes[i];
		}
	}

	float t = 0;
	for (int i = 0; i < this->child_nodes.Num(); i++) {
		t += this->child_nodes[i]->n;
	}
	TArray<float> ucb1_values;
	for (int i = 0; i < this->child_nodes.Num(); i++) {
		Node* node = this->child_nodes[i];
		ucb1_values.Add(-node->w / node->n + (2 * log(t) / node->n) * 0.5);
	}

	return this->child_nodes[this->argmax(&ucb1_values)];
}

float Node::playout(State* _state)
{
	if (_state->isLose()) {
		return -1;
	}

	if (_state->isDraw()) {
		return 0;
	}

	TArray<int32>* smart_legal_actions = _state->smartLegalActions();
	_state->next_void((*smart_legal_actions)[
		FMath::RandRange(0, smart_legal_actions->Num() - 1)]);

	delete smart_legal_actions;

	return -playout(_state);
}

int Node::argmax(TArray<float> * float_array)
{
	int maxindex = 0;
	float maxvalue = 0.f;

	for (int i = 0; i < float_array->Num(); i++) {
		if (maxvalue < (*float_array)[i]) {
			maxvalue = (*float_array)[i];
			maxindex = i;
		}
	}

	return maxindex;
}
