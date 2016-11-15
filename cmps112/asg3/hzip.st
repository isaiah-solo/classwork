#!/afs/cats.ucsc.edu/courses/cmps112-wm/usr/smalltalk-3.2.5/bin/gst -f

"
------------------
Isaiah Solomon
icsolomo
------------------
"

"
------------------
Stream
------------------
"

Object subclass: Stream [
    |stringIndex index byte currentStream writeIndex writeByte|
    Stream class >> new: fileStream [
        |result|
        result := super new.
        result init: fileStream.
        ^result
    ]
    init: fileStream [
        currentStream := fileStream.
        index := 1.
        writeIndex := 9.
        writeByte := 0.
    ]
    nextBit [
        index = 1 ifTrue: [
            byte := currentStream next.
            index := 9.
        ].
        index := index - 1.
        ^byte value bitAt: index
    ]
    putBit: newBit [
        writeIndex := writeIndex - 1.
        writeByte := writeByte bitAt: writeIndex put: newBit.
        writeIndex = 1 ifTrue: [
            currentStream nextPutByte: writeByte.
            writeByte := 0.
            writeIndex := 9.
        ].
    ]
   
    putString: newString [
        newString do: [:index|
            (index asString = '0')
            ifTrue: [self putBit: 0]
            ifFalse: [self putBit: 1].
        ].
    ]
    padOut [
        [writeIndex > 1 & writeIndex < 9] whileFalse: [ 
            self putBit: 0. 
            writeIndex := writeIndex - 1.
        ].
    ]
    atEnd [
            ^index = 1 and: [currentStream atEnd]
    ]
]

"
------------------
NewArray
------------------
"

Object subclass: NewArray [
    |theArray|

    NewArray class >> new [
        self shouldNotImplement.
    ]

    NewArray class >> new: size [
        |result|
        result := super new.
        result init: size.
        ^ result
    ]

    init: size [
        theArray := Array new: size.
    ]

    size [
        ^ theArray size.
    ]

    at: index [
        ^ theArray at: index + 1.
    ]

    at: index put: value [
        ^ theArray at: index + 1 put: value.
    ]

    increment: index [
        (self at: index) isNil
            ifTrue: [ self at: index put: 0].
        self at: index put: (self at: index) + 1.
    ]

    keysAndValuesDo: aBlock [
        (0 to: (self size) - 1) do: [:index |
            aBlock value: index value: (self at: index).
        ]
    ]
]

"
------------------
FileStream
------------------
"

FileStream extend [
    newline := Character newline.
    catFile [
        |count|
        count := 0.
        [self atEnd not] whileTrue: [
            count := count +1.
            count asString size to: 5 do: [:skip| stdout << ' '].
            stdout << count << ': ' << self nextLine << newline.
        ]
    ]
    parseFile [
        |newFrequencyTable|
        newFrequencyTable := NewArray new: 257.
        [self atEnd not] whileTrue: [
            |currentCharacter|
            currentCharacter := self next asInteger.
            newFrequencyTable increment: currentCharacter.
        ].
        newFrequencyTable increment: 256.
        ^ newFrequencyTable
    ]
]

"
------------------
Leaf
------------------
"

Object subclass: Leaf [
    |char count|
    char [ ^ char ]
    count [ ^ count ]
    isLeaf [ ^ true ]
    Leaf class >> new [
        self shouldNotImplement
    ]

    Leaf class >> new: newChar count: newCount [
        |result|
        result := super new.
        result setChar: newChar andCount: newCount.
        ^result
    ]

    setChar: newChar andCount: newCount [
        char := newChar.
        count := newCount.
    ]

    <= other [
        ^ (count < other count)
        | ((count = other count) & (char <= other char)) 
    ]

    printBase: theStream [
        ^ theStream << self class << '(' << char << ',' << count
    ]
    printOn: theStream [
        (self printBase: theStream) << ')'.
    ]

    inOrder: visitor prefix: string [
        visitor value: char value: string.
    ]

    binTreeDump: theStream [
        theStream putBit: 0.
        (1 to: 8) do: [:index | 
            theStream putBit: ( (self char asInteger) bitAt: index) ].
        char asInteger = 0 ifTrue: [
            theStream putBit: 0.
        ].
        char asInteger = 256 ifTrue: [
            theStream putBit: 1.
        ]
    ]
]

"
------------------
Tree
------------------
"

Leaf subclass: Tree [
    |left right|
    Tree class >> new: newChar count: newCount left: aLeft
                  right: aRight [
        |result|
        result := super new: newChar count: newCount.
        result setLeft: aLeft andRight: aRight.
        ^ result
    ]

    isLeaf [ ^ false ]

    setLeft: aLeft andRight: aRight [
        left := aLeft.
        right := aRight.
    ]
    right [ ^ right ]
    left [ ^ left ]

    printOn: theStream [
        (self printBase: theStream) << ',' << left << ','
                                    << right << ')'.
    ]

    inOrder: visitor prefix: string [
        left inOrder: visitor prefix: string, '0'.
        right inOrder: visitor prefix: string, '1'.
    ]

    binTreeDump: theStream [
        left binTreeDump: theStream.
        right binTreeDump: theStream.
        theStream putBit: 1.
    ]
]

newline := Character newline.

dFlag := false.
cFlag := false.
tFlag := false.
uFlag := false.

inputFile := ''.
outputFile := stdout.

(1 to: Smalltalk arguments size)
do: [:index |
    args := (Smalltalk arguments at: index).
    (args includes: $-) ifTrue: [
        (args includes: $d) ifTrue: [dFlag := true.].
        (args includes: $c) ifTrue: [cFlag := true.].
        (args includes: $t) ifTrue: [tFlag := true.].
        (args includes: $u) ifTrue: [uFlag := true.].
    ] ifFalse: [
        (inputFile isEmpty) ifTrue: [
            inputFile := args.
        ] ifFalse: [
            outputFile :=  args.
        ]
    ]
].

"
------------------
Debug Flag
------------------
"

dFlag ifTrue: [
    'Debug on' display.
    newline display.
    tFlag ifTrue: [
        'Decoding tree will be printed' display.
        newline display.
    ]. 
    cFlag ifTrue: [
        'Compressing: ' display.
        inputFile display.
        (outputFile isEmpty) ifFalse: [
            ' to file ' display.
            outputFile display.
            newline display.
        ] ifTrue: [
            ' to stdout' display.
            newline display.
        ]
    ].
    uFlag ifTrue: [
        'Uncompressing: ' display.
        inputFile display.
        (outputFile isEmpty) ifFalse: [
            ' to file ' display.
            outputFile display.
            newline display.
        ] ifTrue: [
            ' to stdout' display.
            newline display.
        ]
    ].    
].

"
------------------
Read File and Write File Streams
------------------
"

|readFile writeFile|
readFile := FileStream open: inputFile 
                       mode: FileStream read.
writeFile := FileStream open: outputFile
                        mode: FileStream write.
"
------------------
Frequency Table
------------------
"
|frequencyTable|
uFlag ifFalse: [frequencyTable := readFile parseFile.].
dFlag ifTrue: [
    (0 to: frequencyTable size - 1)
    do: [:index |
        index display.
        ' ' display.
        item := frequencyTable at: index.
        item display.
        newline display.
    ].
].
|collection|
collection := SortedCollection new.

uFlag ifFalse:[
    (0 to: frequencyTable size - 1)
    do: [:index |
        |aLeaf newCount newChar|
        newCount := frequencyTable at: index.
        newCount isInteger ifTrue: [
            newChar := index asCharacter.
            aLeaf := Leaf new: newChar  count: newCount.
            collection add: aLeaf.
        ].
    ].
].
dFlag ifTrue: [collection inspect.].

uFlag ifFalse: [
    [collection size > 1] whileTrue: [
        |last secondToLast aTree treeChar treeCount|
        last := collection removeFirst.
        secondToLast := collection removeFirst.
        treeChar := last char.
        treeCount := last count + secondToLast count.
        aTree := Tree new: treeChar count: treeCount left: last 
                      right: secondToLast.
        collection add: aTree.
    ].
].
dFlag ifTrue: [collection inspect.].
|theTree encodeTable|
uFlag ifFalse: [ theTree := collection removeLast. ].
encodeTable := NewArray new: 257.
uFlag ifFalse: [
    theTree inOrder: [:char :string |
        |index count|
        index := char asInteger.
        encodeTable at: index put: string.
    ] prefix: ''.
].
tFlag ifTrue: [ "Dumping to stdout"
    (0 to: encodeTable size - 1)
    do: [:index |
        |char great32 less127 count string|
        great32 := index > 32.
        less127 := index < 127.
        count := frequencyTable at: index.
        string := encodeTable at: index.
        count isInteger ifTrue: [
            great32 & less127 ifTrue: [
                char := index asCharacter.
                stdout << char << ' ' << count << ' '
                       << string << newline.
            ] ifFalse: [
                stdout << index << ' ' << count << ' '
                       << string << newline.
            ].
        ].
    ]. 
    readFile close.
].
cFlag ifTrue: [ "Compressing."
    |encryptedString|
    theStream := Stream new: writeFile.
    readFile reset.
    theTree binTreeDump: theStream.
    theStream putBit: 1.
    [readFile atEnd not] whileTrue: [
        |index|
        index := readFile next asInteger.
        encryptedString := (encodeTable at: index).
        theStream putString: encryptedString.
    ].
    encryptedString := encodeTable at: 256.
    theStream putString: encryptedString.
    theStream padOut.
    readFile close.
    writeFile close.
].

uFlag ifTrue: [ "Decompressing."
    outStream := Stream new: readFile.
    inStream := Stream new: writeFile.
   
    stack := OrderedCollection new.
    treeComplete := false.
    decodeTree := nil.

    [(outStream atEnd not) & (treeComplete = false)] whileTrue: [
        |currentCharacter|
        currentCharacter := outStream nextBit.
        currentCharacter = 0 ifTrue: [
            realChar := 0.
            baseOfTwo := 1.

            (1 to: 8) do: [:skip |
                |currentBit|
                currentBit := outStream nextBit.
                (currentBit = 1)
                ifTrue: [realChar := realChar + baseOfTwo.].
                baseOfTwo := baseOfTwo *2.
            ].
            realChar = 0 ifTrue: [
                |currentBit|
                currentBit := outStream nextBit.
                currentBit =1 ifTrue: [
                    stack addFirst:
                        (Leaf new: 256 asCharacter count: 1).
                ]
                ifFalse: [
                    stack addFirst:
                        (Leaf new: 0 asCharacter count: 1).
                ].
            ] ifFalse: [
                stack addFirst:
                    (Leaf new: realChar asCharacter count: 1).
            ].
        ] ifFalse: [
            stack size > 0 ifTrue: [
                right := stack removeFirst.
                stack size = 0 ifTrue: [
                    decodeTree := right.
                    treeComplete := true.
                ] ifFalse: [
                    left := stack removeFirst.
                    stack addFirst: (Tree new: (left char) count: 1
                                          left: left right: right).
                ].
            ].
        ].
    ].
    dFlag ifTrue: [decodeTree inspect.].

    temporaryTree := decodeTree.
    eof := false.
    [(outStream atEnd not) & (eof = false)] whileTrue: [
        |currentBit|

        currentBit := outStream nextBit.

        currentBit = 0 ifTrue: [
            temporaryTree isLeaf ifTrue: [
                temporaryTree char asInteger =
                    256 ifTrue: [eof = true.].
            ] ifFalse: [
                temporaryTree := temporaryTree left.
                temporaryTree  isLeaf ifTrue: [
                    temporaryTree char asInteger =
                        256 ifTrue: [ eof = true.]
                    ifFalse: [
                        writeFile << temporaryTree char.
                    ].
                    temporaryTree := decodeTree.
                ].
            ].
        ] ifFalse: [
            temporaryTree isLeaf ifTrue: [
                temporaryTree char asInteger =
                    256 ifTrue: [eof := true.].
            ] ifFalse: [
                temporaryTree = temporaryTree right.
                temporaryTree  isLeaf ifTrue: [
                    temporaryTree char asInteger =
                        256 ifTrue: [ eof := true.]
                    ifFalse: [
                        writeFile << temporaryTree char.
                    ].
                    temporaryTree := decodeTree.
                ].
            ].
        ].
    ].

    readFile close.
    writeFile close.
].
