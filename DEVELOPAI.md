# How to Build AIs

You may choose to write your AI in either Python or C++. C++ would be more performant, but the choice is yours!

## C++ Method

Create your `<bot>.cpp` and `<bot>.h` in the `aicontrollers/` directory.

Add both files to the `aicontrollers/CMakeLists.txt` like how the other files are added (`"${DIR}/<bot>[.cpp/.h]"`).

Your class needs to extend `public Mahjong::PlayerController` (which can be included with `#include <playerconroller.h>`) and all virtual functions of `PlayerController` need to be implemented. See `aicontroller/angrydiscardobot.h` for an example of this. Keep data variables that needs to be stored on the AI `private`.

Register your bot by adding an initializer lambda to the `avaliableControllers` map in `src/manager.cpp`. The map entry should look like the following:

```
{"BotName",[]() -> PlayerController* {return new BotClass;}},
```

Note how the lambda returns a pointer to the class, not an instance of it. This is intentional.

## Python Method

Create a file for your ai bot in `riichiroyale/game/<bot>.py` and add `from .<bot> import *` to the `riichiroyale/game/__init__.py` file.

Create your AI class, it should extend `MahjongAI` and `Player` (import `MahjongAI` from `libmahjong` and import `Player` from `.player`).

The beginning of your `def __init__(self)` should look like the following:

```
class YourBot(MahjongAI, Player):
  def __init__(self):
    MahjongAI.__init__(self)
    Player.__init__(self, self.Name())
    ...continue rest of initializer here...
```

Register your class as an avaliable AI. This is done by creating an instance of your python class and passing it to `register_ai` as follows:

```
my_bot = BotClass()
register_ai(my_bot, "MyBot")
```

Put this within the `# Register Python AIs` section.

# Bot Functions

## `Name()`

A static function that returns the name of the bot as a string. Please keep this name consistant.

## `GameStart(int id)`

Called when the game is started. The `id` passed is the unique ID of your player for the game.

## `RoundStart(Piece[] hand, Wind seatWind, Wind prevalentWind)`

Called when a new round is started. `hand` is the list of tiles in your starting hand. `seatWind` is your current seat's wind. `prevalentWind` is the rounds prevalent wind.

## `ReceiveEvent(Event e)`

Called when an event occurs. Events are comprised of the following:

```
Event:
  EventType type
  int playerID
  int piece
  bool decision
```

See `event.h` for a list of all events. See `piecetype.h` for a list of all piece types.

If `decision` is true, expect `RetrieveDecision()` to be called soon.

### How to interpret `decision == true`

If decision is true, the information in the event provides insight into what exactly you're making the decision on. For example, in the case of a Pon event with `decision` true, the `playerID` provided is who you would be calling on, and `piece` is the piece you would gain for your meld.

In the case of a Discard event with `decision` true, the `piece` provided is the piece that was drawn to your hand.

### How to interpret `decision == false`

If decision is false, the information in the event provides insight into the official standings of the game.

After every successful decision you make, **the game will provide you with an event of the decision you just took, but with `decision` false.** This is known as the *decision receipt*, and should be used as your source of truth on the current standings of your player in the game.

If `decision` is false and the `playerID` is not your AIs id, this means the event is informing you of new public data, such as the other player calling Pon or discarding a piece.

## `RetrieveDecision`

Called when the game loop requires an event to be returned, representing your decision on the previously sent decision.

### How Decisions are Communicated

Given a decision (such as a Pon, Chi, Riichi, Discard, etc), returning an event with the same type will indicate choosing that decision. For decisions that require additional information, such as a specific tile, the `piece` attribute on the event is used.

If the decision you returned is invalid (an event type you weren't asked about, discarding a piece you don't have, etc), it will ask about the same decision again. Note that in this case, `RetrieveDecision` is simply called again, no new event is received.
