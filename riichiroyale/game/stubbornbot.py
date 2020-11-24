import random

import libmahjong
from libmahjong.libmahjong import MahjongAI, EventType, EngineEvent, Piece, PieceType, GREEN_DRAGON
from libmahjong.libmahjong import ERROR, TERMINAL_BIT, CHARACTER_SUIT, HONOR_SUIT

from .player import Player
from .gameevent import Event


class StubbornBot(MahjongAI, Player):
    def __init__(self):
        MahjongAI.__init__(self)
        Player.__init__(self, self.Name())
        self.decision = None
        self.decision_to_act_on = None
        self.others_discarded_pieces = []

    def Name(self):
        return "StubbornBot"

    def GameStart(self, arg0):
        self.player_id = arg0
        print("Assigned player with id", self.player_id)

    def RoundStart(self, hand, seatWind, prevalentWind):
        self.hand = hand
        for index, piece in enumerate(hand):
            hand[index] = piece.get_raw_value()
        self.seat_wind = seatWind
        self.prevalent_wind = prevalentWind
        self.goal_yaku = self.ChooseGoalYaku()

    def ReceiveEvent(self, e):
        if e.decision:
            self.decision = e
            self.decision_to_act_on = e
            if e.type == EventType.Discard:
                print("player", self.player_id, "PIECE TYPE:", e.piece)
        else:
            if e.type == EventType.Discard and e.player != self.player_id:
                self.others_discarded_pieces.append(e.piece)

    def __isHonor(self, p):  # These functions aren't exported from the C++ code for some reason
        if type(p) == int:
            return (p & int(CHARACTER_SUIT)) == HONOR_SUIT
        return (p.get_raw_value() & int(CHARACTER_SUIT)) == HONOR_SUIT

    def __isTerminal(self, p):
        if type(p) == int:
            return (p & int(TERMINAL_BIT)) != ERROR
        return (p.get_raw_value() & int(TERMINAL_BIT)) != ERROR


    def CountTerminalsAndHonors(self):
        i = 0
        for piece in self.hand:
            if self.__isHonor(piece) or self.__isTerminal(piece):
                i += 1
        return i

    def ChooseGoalYaku(self):
        options = ["riichi", "outside"]
        if self.CountTerminalsAndHonors() < 5:
            return "all_simples"
        else:
            return random.choice(options)


    def GetPieceValue(self, piece):  # Gets the value of a piece, given the player's hand
        suits = [PieceType.BAMBOO_SUIT, PieceType.CHARACTER_SUIT, PieceType.PIN_SUIT]
        dragon_suits = [PieceType.GREEN_DRAGON, PieceType.RED_DRAGON, PieceType.WHITE_DRAGON]

        value = 0
        piece_temp = Piece(piece)

        for index, suit in enumerate(dragon_suits):
            if piece == suit:  # if piece is a dragon
                if self.CountInHand(PieceType.HONOR_SUIT, 5 + index) == 3:  # If piece forms a triplet of dragons
                    value += 1

        if piece == self.seat_wind or piece == self.prevalent_wind:
            value += 1

        if self.CountInHand(piece_temp.get_suit(), piece_temp.get_piece_num()) == 3:  # If piece forms a pon
            value += 1

        # TODO: find sequences

        if piece_temp.is_red_five():
            value += 1

        return value


    def PickDiscard(self):
        if self.goal_yaku == "riichi":

            lowestIndex = 0
            lowestValue = 0
            for index, piece in enumerate(self.hand):
                if self.GetPieceValue(piece) > lowestValue:
                    lowestIndex = index
                    lowestValue = self.GetPieceValue(piece)

            pieceToDiscard = self.hand[lowestIndex]
            self.decision.piece = pieceToDiscard
            print("Discarding:", self.decision.piece)
            return self.decision

        elif self.goal_yaku == "all_simples":
            if self.__isHonor(self.decision_to_act_on.piece) or self.__isTerminal(self.decision_to_act_on.piece):
                print("Discarding:", self.decision_to_act_on.piece)
                return self.decision  # Discard piece we just picked
            else:
                for piece in self.hand:
                    if self.__isHonor(self.decision_to_act_on.piece) or self.__isTerminal(self.decision_to_act_on.piece):
                        self.decision.piece = piece
                        print("Discarding:", self.decision.piece)
                        return self.decision  # Discard first honor or terminal piece we find

                # If we got here, then our hand has no more honors or terminals
                lowestIndex = 0
                lowestValue = 0
                for index, piece in enumerate(self.hand):
                    if self.GetPieceValue(piece) > lowestValue:
                        lowestIndex = index
                        lowestValue = self.GetPieceValue(piece)
                pieceToDiscard = self.hand[lowestIndex]
                self.decision.piece = pieceToDiscard

                print("Discarding:", self.decision.piece)
                return self.decision
        elif self.goal_yaku == "outside":
            if not (self.__isHonor(self.decision_to_act_on.piece) or self.__isTerminal(self.decision_to_act_on.piece)):
                if not (Piece(self.decision_to_act_on.piece).get_piece_num() <= 3 or Piece(self.decision_to_act_on.piece).get_piece_num() >= 7):
                    print("Discarding:", self.decision.piece)
                    return self.decision  # Discard piece we just picked
            for piece in self.hand:
                if not (self.__isHonor(self.decision_to_act_on.piece) or self.__isTerminal(self.decision_to_act_on.piece)):
                    if not (Piece(piece).get_piece_num() <= 3 or Piece(piece).get_piece_num() >= 7):
                        self.decision.piece = piece
                        print("Discarding:", self.decision.piece)
                        return self.decision  # Discard piece we just picked

            # If we got here, then our hand has no more non-terminal pieces
            lowestIndex = 0
            lowestValue = 0
            for index, piece in enumerate(self.hand):
                if self.GetPieceValue(piece) > lowestValue:
                    lowestIndex = index
                    lowestValue = self.GetPieceValue(piece)
            pieceToDiscard = self.hand[lowestIndex]
            self.decision.piece = pieceToDiscard

            print("Discarding:", self.decision.piece)
            return self.decision
        else:
            print(self.goal_yaku)

    def CountInHand(self, suit, number):
        i = 0
        for piece in self.hand:
            if piece == int(suit) | number:
                i += 1
        return i


    def HandContains(self, suit, number):
        for piece in self.hand:
            if piece == int(suit) | number:
                return True
        return False

    def DecideChi(self):
        if self.goal_yaku == "riichi":
            self.decision.type = EventType.Decline
            return self.decision  # Don't make calls
        elif self.goal_yaku == "all_simples":
            if self.__isHonor(self.decision_to_act_on.piece) or self.__isTerminal(self.decision_to_act_on.piece):
                self.decision.type = EventType.Decline
                return self.decision  # Don't make calls on honors or terminals

            suits = [PieceType.BAMBOO_SUIT, PieceType.CHARACTER_SUIT, PieceType.PIN_SUIT]
            for suit in suits:
                if Piece(self.decision_to_act_on.piece).get_piece_num() == 7:
                    if self.HandContains(suit, 8) and self.HandContains(suit, 9):
                        self.decision.type = EventType.Decline
                        return self.decision  # Don't chi if it'll include a 9 (terminal)
                elif Piece(self.decision_to_act_on.piece).get_piece_num() == 8:
                    if self.HandContains(suit, 7) and self.HandContains(suit, 9):
                        self.decision.type = EventType.Decline
                        return self.decision  # Don't chi if it'll include a 9 (terminal)
            return self.decision  # Make the call if all conditions are met


        # TODO
        elif self.goal_yaku == "outside":
            if self.__isHonor(self.decision_to_act_on.piece) or self.__isTerminal(self.decision_to_act_on.piece):
                return self.decision
            else:
                self.decision.type = EventType.Decline
                return self.decision

    def DecidePon(self):
        if self.goal_yaku == "riichi":
            self.decision.type = EventType.Decline
            return self.decision  # Don't make calls
        elif self.goal_yaku == "all_simples":
            if self.__isHonor(self.decision.piece) or self.__isTerminal(self.decision.piece):
                self.decision.type = EventType.Decline
                return self.decision  # Don't make calls on honors or terminals
            else:
                return self.decision
        elif self.goal_yaku == "outside":
            if self.__isHonor(self.decision.piece) or self.__isTerminal(self.decision.piece):
                return self.decision
            else:
                self.decision.type = EventType.Decline
                return self.decision

    # TODO
    def DecideKan(self):
        return self.DecidePon()

    def RetrieveDecision(self):  # Seems to fail if the player already has the same piece in their hand as the one they just drew

        if self.decision_to_act_on.type == EventType.Discard:

            self.hand.append(self.decision_to_act_on.piece)
            print("Hand after picking:", self.hand)
            print("Picking tile to discard for player", self.player_id, " drew piece:", self.decision_to_act_on.piece)
            self.decision = self.PickDiscard()
            self.hand.remove(self.decision.piece)
            print("Hand after discarding:", self.hand)

        elif self.decision_to_act_on.type == EventType.Pon:
            self.decision = self.DecidePon()

        elif self.decision_to_act_on.type == EventType.Chi:
            self.decision = self.DecideChi()

        elif self.decision_to_act_on.type == EventType.Kan:
            self.decision = self.DecideKan()
        else:
            print("Requested a decision for an unimplemented event type:", self.decision.type)

        return self.decision

