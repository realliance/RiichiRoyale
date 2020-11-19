import random
from threading import Lock

from libmahjong.libmahjong import PieceType
from libmahjong.libmahjong import MahjongAI, EventType, EngineEvent
from .player import Player
from .gameevent import Event

class StubbornBot(MahjongAI, Player):
    def __init__(self):
        MahjongAI.__init__(self)
        Player.__init__(self, self.Name())
        self.decision = None
        self.others_discarded_pieces = []

    def Name(self):
        return "StubbornBot"

    def GameStart(self, arg0):
        self.player_id = arg0

    def RoundStart(self, hand, seatWind, prevalentWind):
        self.hand = hand
        self.seat_wind = seatWind
        self.prevalent_wind = prevalentWind
        self.goal_yaku = self.ChooseGoalYaku(self.hand)

    def ReceiveEvent(self, e):
        if e.decision:
            self.decision = e
        else:
            if e.type == EventType.Discard and e.player != self.player_id:
                self.others_discarded_pieces.append(e.piece)

    def ChooseGoalYaku(self, hand):
        return random.choice(["riichi", "all_simples", "outside"])



    def PickDiscard(self):
        if self.goal_yaku == "riichi":
            print("TODO 0")
        elif self.goal_yaku == "all_simples":
            if self.decision.piece.isHonor() or self.decision.piece.isTerminal():
                return self.decision  # Discard piece we just picked
            else:
                for piece in self.hand:
                    if piece.isHonor() or piece.isTerminal():
                        self.decision.piece = piece
                        return self.decision  # Discard first honor or terminal piece we find
                # If we got here, then our hand has no more honors or terminals
                print("TODO 1")
                return self.decision
        elif self.goal_yaku == "outside":
            if not (self.decision.piece.isHonor() or self.decision.piece.isTerminal()):
                if not (self.decision.piece.get_piece_num() <= 3 or self.decision.piece.get_piece_num() >= 7):
                    return self.decision  # Discard piece we just picked
            for piece in self.hand:
                if not (piece.isHonor() or piece.isTerminal()):
                    if not (piece.get_piece_num() <= 3 or piece.get_piece_num() >= 7):
                        self.decision.piece = piece
                        return self.decision  # Discard piece we just picked
            print("TODO 2")
            return self.decision

    def HandContains(self, suit, number):
        for piece in self.hand:
            if piece == suit | number:
                return True
        return False

    def DecideChi(self):
        if self.goal_yaku == "riichi":
            self.decision.type = EventType.Decline
            return self.decision  # Don't make calls
        elif self.goal_yaku == "all_simples":
            if self.decision.piece.isHonor() or self.decision.piece.isTerminal():
                self.decision.type = EventType.Decline
                return self.decision  # Don't make calls on honors or terminals

            suits = [PieceType.BAMBOO_SUIT, PieceType.CHARACTER_SUIT, PieceType.PIN_SUIT]
            for suit in suits:
                if self.decision.piece.get_piece_num() == 7:
                    if self.HandContains(suit, 8) and self.HandContains(suit, 9):
                        self.decision.type = EventType.Decline
                        return self.decision  # Don't chi if it'll include a 9 (terminal)
                elif self.decision.piece.get_piece_num() == 8:
                    if self.HandContains(suit, 7) and self.HandContains(suit, 9):
                        self.decision.type = EventType.Decline
                        return self.decision  # Don't chi if it'll include a 9 (terminal)
            return self.decision  # Make the call if all conditions are met


        # TODO
        elif self.goal_yaku == "outside":
            if self.decision.piece.isHonor() or self.decision.piece.isTerminal():
                return self.decision
            else:
                self.decision.type = EventType.Decline
                return self.decision

    def DecidePon(self):
        if self.goal_yaku == "riichi":
            self.decision.type = EventType.Decline
            return self.decision  # Don't make calls
        elif self.goal_yaku == "all_simples":
            if self.decision.piece.isHonor() or self.decision.piece.isTerminal():
                self.decision.type = EventType.Decline
                return self.decision  # Don't make calls on honors or terminals
            else:
                return self.decision
        elif self.goal_yaku == "outside":
            if self.decision.piece.isHonor() or self.decision.piece.isTerminal():
                return self.decision
            else:
                self.decision.type = EventType.Decline
                return self.decision

    # TODO 
    def DecideKan(self):
        return self.DecidePon()

    def RetrieveDecision(self):
        if self.decision.type == EventType.Discard:
            self.hand.append(self.decision.piece)
            self.decision = self.PickDiscard()
        elif self.decision.type == EventType.Pon:
            self.decision = self.DecidePon()
        elif self.decision.type == EventType.Chi:
            self.decision = self.DecideChi()
        elif self.decision.type == EventType.Kan:
            self.decision = self.DecideKan()
        else:
            print("Requested a decision for an unimplemented event type:", self.decision.type)

        return self.decision

