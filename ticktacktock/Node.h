// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State.h"

/**
 * 
 */
class TICKTACKTOCK_API Node
{
public:
	State* state;	// 상태
	float w;		// 가치 누계
	int n;			// 시행 횟수
	TArray<Node*> child_nodes;

public:
	Node(State * state);
	~Node();

	void expand(void);
	float evaluate(void);
	Node* next_child_node(void);
	float playout(State* state);

	int argmax(TArray<float> * float_array);
};
