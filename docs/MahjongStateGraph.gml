graph [
  version 2
  directed 1
  rankdir "LR"
  node [
    id 0
    label "GameStart"
    rank "source"
  ]
  node [
    id 1
    label "GameEnd"
    rank "sink"
  ]
  node [
    id 2
    label "Kan"
    rank ""
  ]
  node [
    id 3
    label "KanDiscard"
    rank ""
  ]
  node [
    id 4
    label "ConcealedKan"
    rank ""
  ]
  node [
    id 5
    label "ConvertedKan"
    rank ""
  ]
  node [
    id 6
    label "Replacement"
    rank ""
  ]
  node [
    id 7
    label "Ron"
    rank ""
  ]
  node [
    id 8
    label "Draw"
    rank ""
  ]
  node [
    id 9
    label "PlayerHand"
    rank ""
  ]
  node [
    id 10
    label "Pon"
    rank ""
  ]
  node [
    id 11
    label "Discard"
    rank ""
  ]
  node [
    id 12
    label "Chi"
    rank ""
  ]
  node [
    id 13
    label "Riichi"
    rank ""
  ]
  node [
    id 14
    label "Exhaust"
    rank ""
  ]
  node [
    id 15
    label "RoundEnd"
    rank ""
  ]
  node [
    id 16
    label "Tsumo"
    rank ""
  ]
  node [
    id 17
    label "RoundStart"
    rank ""
  ]
  edge [
    id 26
    source 0
    target 17
  ]
  edge [
    id 1
    source 2
    target 3
  ]
  edge [
    id 4
    source 3
    target 6
  ]
  edge [
    id 5
    source 3
    target 7
  ]
  edge [
    id 2
    source 4
    target 3
  ]
  edge [
    id 3
    source 5
    target 3
  ]
  edge [
    id 7
    source 6
    target 9
  ]
  edge [
    id 13
    source 7
    target 15
  ]
  edge [
    id 6
    source 8
    target 9
  ]
  edge [
    id 18
    source 9
    target 4
  ]
  edge [
    id 19
    source 9
    target 5
  ]
  edge [
    id 10
    source 9
    target 11
  ]
  edge [
    id 20
    source 9
    target 13
  ]
  edge [
    id 17
    source 9
    target 16
  ]
  edge [
    id 8
    source 10
    target 11
  ]
  edge [
    id 23
    source 11
    target 2
  ]
  edge [
    id 21
    source 11
    target 7
  ]
  edge [
    id 28
    source 11
    target 8
  ]
  edge [
    id 24
    source 11
    target 10
  ]
  edge [
    id 25
    source 11
    target 12
  ]
  edge [
    id 22
    source 11
    target 14
  ]
  edge [
    id 9
    source 12
    target 11
  ]
  edge [
    id 11
    source 13
    target 11
  ]
  edge [
    id 12
    source 14
    target 15
  ]
  edge [
    id 16
    source 15
    target 1
  ]
  edge [
    id 15
    source 15
    target 17
  ]
  edge [
    id 14
    source 16
    target 15
  ]
  edge [
    id 27
    source 17
    target 8
  ]
]
