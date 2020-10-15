#pragma once
#include <algorithm>
#include <fstream>
#include <iostream>

#include "handnode.h"
#include "piecetype.h"
#include "walls.h"
#include "hand.h"



bool possibleChi(uint8_t* counts, Piece p);

bool possiblePair(uint8_t* counts, Piece p);

bool possiblePon(uint8_t* counts, Piece p);

void countPieces(uint8_t* counts, const std::vector<Piece>& pieces);

void createNodes(uint8_t* counts, const std::vector<Piece>& pieces, std::vector<Piece>& pairs, std::vector<Piece>& pon, std::vector<Piece>& chi, std::vector<Piece>& singles);

auto breakdownHand(std::vector<Piece> pieces) -> Node*;
