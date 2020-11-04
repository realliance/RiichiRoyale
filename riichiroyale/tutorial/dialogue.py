dialogue = {
    "intro": [
        "Welcome to the tutorial for RiichiRoyale! I’m here to teach you some of the basic rules and mechanics of riichi mahjong, and how to apply them.",
        "The basic goal of riichi mahjong is to form a hand consisting of 4 triplets and one pair: 14 tiles in total. There are two types of triplets: pon and chi. Pons are triplets where you have 3 of the same exact tile. Chis are triplets consisting of a sequence. For example having the 3, 4, and 5 of bamboo is a chi.",
        "Note: Chis are only possible to form with numbered tiles, so there is no valid chi consisting of dragons or winds.",
        "In addition to these two common types of triplets, there is a 3rd type, called a kan. A kan consists of 4 of the same exact tile. However, calling a kan will effectively ignore the 4th piece, so it is equivalent of having a pon. You also draw a replacement tile after calling a kan to be able to complete a hand.",
        "If you are waiting for a tile to complete a pon, chi, or kan, and another player discards that piece, you can take that piece and add it to your hand. Doing this will then create a ‘melded’ chi pon or kan, that is set aside. Pieces in a melded set cannot be discarded, and are essentially stuck there until the end of the round.",
        "When another player discards the last tile you need to win, you can call ron to end the game using that tile. If instead you draw your winning tile, you call tsumo to win.",
        "There are some extra rules that are important to keep in mind, but we’ll get to them as they become relevant. Let’s go through a quick example game to apply some of what we’ve learned.",
        "Players start with 13 tiles in their hands. A turn consists of drawing a 14th tile, and discarding a tile. Your newly drawn tile will appear on the right. Looking at your hand, you have a lot of potential triplets. You also just drew a second red dragon. Now would be a good time to explain yakkus.",
        "In addition to making a 14 tile hand (4 triplets and a pair), in order to win a round, your hand must satisfy at least one win condition (yakku). There are too many different yakkus to explain them all here, so I would recommend looking up a list of the yakkus and learning some of the easier ones. In the case of your hand however, having two red dragons sets you up for a dragon triplet yakku.",
        "As you might guess, this involves having a triplet of dragons. All you need to wait for now is someone to discard a red dragon. For now, you should discard the south wind, since it’s fairly useless to you. Click on the South Wind to discard it.",
    ],
    "pon": ["There’s the third red dragon! Click pon to meld it into your hand."],
    "after_pon": [
        "A few things to note. After making a call that melds a tile into your hand, it becomes your turn and you must discard. Additionally, now that you’ve melded, you have an ‘open’ hand. Having an open hand only really affects what yakkus you can go after, but it doesn’t affect the dragon triple yakku.",
        "Looking at your hand now, the 5 and 6 of characters are valuable since a 4 or a 7 would make a triple. The 5 and 7 of pins can also make a triple, but only if you get a 6. The one of bamboo is good to discard since terminal pieces (1s and 9s) are typically harder to expand off of than others.",
    ],
    "chi": [
        "Now, if you look at the discards of the players across and to the left of you, they are respectively: a 6 pin and a 5 char. You might be thinking that you should have been able to chi on the 6 pin, to make your 567 triple. However, here’s an additional rule about chi that states that you can only call chi on the player to your left. This is done to make up for the fact that chi’s are statistically more likely to happen than pons. Thus, you cannot call chi on this 6 pin.",
        "But you are able to chi on the 5 char since the player on your left discarded it. Do it.",
    ],
    "after_chi": [
        "If you look at your hand, if you discard the 7 bamboo, you will be one tile away (the 6 pin) from winning. You have two melded triples, a sequence of 2 3 4 of pins, and a pair of 1 of characters.",
        "Being only one tile away from winning is called being in tenpai. When in tenpai, if any player discards your winning tile, you can call ron to claim the tile and win. This means that even though you couldn’t call chi earlier on the 6 pin since the player across from you discarded it, if they were to discard another 6 pin, you would be able to call it now since it’s your winning tile.",
    ],
    "skip_pon": [
        "Even though you are able to pon this 1 character, it wouldn’t help you since you are already one tile away from winning. You should let this one go."
    ],
    "discard_tip": [
        "Here’s a bit of strategy advice. If you were to discard this 4 pin, your wait would be the same: the 6 pin. However, if you were to instead discard the 7 pin, your single wait turns into an open wait of a 6 pin or 3 pin. Because of this it would be wise to discard the 7 pin."
    ],
    "ron": ["There it is! Your winning tile. Call ron to claim it and end this round."],
    "end": ["Congratulations! You won! Click Next to return to the main menu."],
}


def get_dialogue():
    return dialogue
