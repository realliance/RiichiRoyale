from enum import Enum, IntEnum

class Call(Enum):
    Pon = 0
    Chi = 1
    Kan = 2
    Ron = 3
    Concealed_Kan = 4
    Riichi = 5
    Tsumo = 6
    Skip = 7


class CallDirection(IntEnum):
    Left = 1
    Forward = 2
    Right = 3
    Concealed = 0

    @staticmethod
    def get_call_direction(you, opponent):
        if you == 0 and opponent == 3:
            return CallDirection.Left
        if you == 3 and opponent == 0:
            return CallDirection.Right
        return CallDirection(abs(opponent - you))

    @staticmethod
    def should_rotate_tile(i, meld, seat):
        number_of_tiles = len(meld.tiles)
        direction = meld.call_direction
        converted_pon_to_kan = meld.converted_kan

        if direction == CallDirection.Concealed:
            return False

        SEAT_TABLE = [
            # Seat 0
           {
                CallDirection.Left: i in ([0, 1] if converted_pon_to_kan else [0]),
                CallDirection.Forward: i in ([1, 2] if converted_pon_to_kan else [1]),
                CallDirection.Right: i in (
                    [number_of_tiles - 1, number_of_tiles - 2]
                    if converted_pon_to_kan
                    else [number_of_tiles - 1]
                )
            },

            # Seat 1
            {
                CallDirection.Left: i in ([-1, 0] if converted_pon_to_kan else [0]),
                CallDirection.Forward: i in ([0, 1] if converted_pon_to_kan else [1]),
                CallDirection.Right: i in (
                    [number_of_tiles - 1, number_of_tiles - 2]
                    if converted_pon_to_kan
                    else [number_of_tiles - 1]
                )
            },

            # Seat 2
            {
                CallDirection.Left: i in ([-1, 0] if converted_pon_to_kan else [0]),
                CallDirection.Forward: i in ([0, 1] if converted_pon_to_kan else [1]),
                CallDirection.Right: i in (
                    [number_of_tiles - 1, number_of_tiles - 2]
                    if converted_pon_to_kan
                    else [number_of_tiles - 1]
                )
            },


            # Seat 3
            {
                CallDirection.Left: i in ([-1, 0] if converted_pon_to_kan else [0]),
                CallDirection.Forward: i in ([1, 2] if converted_pon_to_kan else [1]),
                CallDirection.Right: i in (
                    [number_of_tiles - 1, number_of_tiles - 2]
                    if converted_pon_to_kan
                    else [number_of_tiles - 1]
                )
            },
        ]

        return SEAT_TABLE[seat][direction]