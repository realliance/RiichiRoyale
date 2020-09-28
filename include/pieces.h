#pragma once

enum Set {
    CONCEALED_KAN,
    KAN,
    CHI,
    PON
};

using Piece = u_int8_t;

// TERMINAL_BIT, SUIT_2, RED_FIVE, PIECE_4
// For honor pieces suit bits are set to 00.

enum Pieces {
    TERMINAL_BIT = 1<<7,
    HONOR_SUIT = 0<<4,
    BAMBOO_SUIT = 1<<4,
    PIN_SUIT = 2<<4,
    CHARACTER_SUIT = 3<<4,
    RED_FIVE = 1<<3,
    ERROR_PIECE = 0,
    ONE_BAMBOO   = BAMBOO_SUIT | 1 | TERMINAL_BIT,
    TWO_BAMBOO   = BAMBOO_SUIT | 2,
    THREE_BAMBOO = BAMBOO_SUIT | 3,
    FOUR_BAMBOO  = BAMBOO_SUIT | 4,
    FIVE_BAMBOO  = BAMBOO_SUIT | 5,
    RED_FIVE_BAMBOO = BAMBOO_SUIT | 5 | RED_FIVE,
    SIX_BAMBOO   = BAMBOO_SUIT | 6,
    SEVEN_BAMBOO = BAMBOO_SUIT | 7,
    EIGHT_BAMBOO = BAMBOO_SUIT | 8,
    NINE_BAMBOO  = BAMBOO_SUIT | 9 | TERMINAL_BIT,
    ONE_PIN   = PIN_SUIT | 1 | TERMINAL_BIT,
    TWO_PIN   = PIN_SUIT | 2,
    THREE_PIN = PIN_SUIT | 3,
    FOUR_PIN  = PIN_SUIT | 4,
    FIVE_PIN  = PIN_SUIT | 5,
    RED_FIVE_PIN = PIN_SUIT | 5 | RED_FIVE,
    SIX_PIN   = PIN_SUIT | 6,
    SEVEN_PIN = PIN_SUIT | 7,
    EIGHT_PIN = PIN_SUIT | 8,
    NINE_PIN  = PIN_SUIT | 9 | TERMINAL_BIT,
    ONE_CHARACTER   = CHARACTER_SUIT | 1 | TERMINAL_BIT,
    TWO_CHARACTER   = CHARACTER_SUIT | 2,
    THREE_CHARACTER = CHARACTER_SUIT | 3,
    FOUR_CHARACTER  = CHARACTER_SUIT | 4,
    FIVE_CHARACTER  = CHARACTER_SUIT | 5,
    RED_FIVE_CHARACTER = CHARACTER_SUIT | 5 | RED_FIVE,
    SIX_CHARACTER   = CHARACTER_SUIT | 6,
    SEVEN_CHARACTER = CHARACTER_SUIT | 7,
    EIGHT_CHARACTER = CHARACTER_SUIT | 8,
    NINE_CHARACTER  = CHARACTER_SUIT | 9 | TERMINAL_BIT,
    EAST_WIND    = HONOR_SUIT | 1,
    SOUTH_WIND   = HONOR_SUIT | 2,
    WEST_WIND    = HONOR_SUIT | 3,
    NORTH_WIND   = HONOR_SUIT | 4,
    RED_DRAGON   = HONOR_SUIT | 5,
    WHITE_DRAGON = HONOR_SUIT | 6,
    GREEN_DRAGON = HONOR_SUIT | 7
};

inline auto isHonor(Piece p) -> bool{
    return (p & CHARACTER_SUIT) == ERROR_PIECE;
}

inline auto isTerminal(Piece p) -> bool{
    return (p & TERMINAL_BIT) != ERROR_PIECE;
}

inline auto getSuit(Piece p) -> int {
    return p & 3<<4;
}

inline auto getPieceNum(Piece p) -> int {
    return p & 15;
}
