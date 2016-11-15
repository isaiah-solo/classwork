#!/afs/cats.ucsc.edu/courses/cmps112-wm/usr/smalltalk-3.2.5/bin/gst -f
"
Isaiah Solomon
icsolomo
"
"
Options:
Exactly one of these options is required
  -d    Debug information is printed for the benefit of the 
        application author
  -t    The compression algorithm is activated, and the decoding tree  
        is printed to the standard output. the output filename may not
        be specified.
  -c    The input file is compressed and written to the output file, if
        specified, or to the standard output, if not.
  -u    The input file is assumed to be compressed, and is uncompressed,
        written to the output file, if specified, or to the std output 
        if not
"

" BitStream "
Object subclass: BitStream [
    |stringIndex bitIndex byte myStream writeIndex writeByte|
    BitStream class >> new: fileStream [
        |result|
        result := super new.
        result init: fileStream.
        ^result
    ]
    init: fileStream [
        myStream := fileStream.
        bitIndex := 1.
        writeIndex := 9.
        writeByte := 0.
    ]
    nextBit [
        bitIndex = 1 ifTrue: [
            byte := myStream next.
            bitIndex := 9.
        ].
        bitIndex := bitIndex - 1.
        ^byte value bitAt: bitIndex
    ]
    putBit: theBit [
        writeIndex := writeIndex - 1.
        writeByte := writeByte bitAt: writeIndex put: theBit.
        writeIndex = 1 ifTrue: [
            myStream nextPutByte: writeByte.
            writeByte := 0.
            writeIndex := 9.
        ].
    ]
   
    putString: theString [
        theString do: [:index|
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
            ^bitIndex = 1 and: [myStream atEnd]
    ]
]

" ZeroArray "
Object subclass: ZeroArray [
    |theArray|

    ZeroArray class >> new [
        self shouldNotImplement.
    ]

    ZeroArray class >> new: size [
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

    incr: index [
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

" FileStream "
FileStream extend [
    nl := Character nl.
    catFile [
        |count|
        count := 0.
        [self atEnd not] whileTrue: [
            count := count +1.
            count asString size to: 5 do: [:skip| stdout << ' '].
            stdout << count << ': ' << self nextLine << nl.
        ]
    ]
    parseFile [
        |freqtable|
        freqtable := ZeroArray new: 257.
        [self atEnd not] whileTrue: [
            |ordChar|
            ordChar := self next asInteger.
            freqtable incr: ordChar.
        ].
        freqtable incr: 256.
        ^ freqtable
    ]
]

" Leaf "
Object subclass: Leaf [
    |char count|
    char [ ^ char ]
    count [ ^ count ]
    isLeaf [ ^ true ]
    Leaf class >> new [
        self shouldNotImplement
    ]

    Leaf class >> new: aChar count: aCount [
        |result|
        result := super new.
        result setChar: aChar andCount: aCount.
        ^result
    ]

    setChar: aChar andCount: aCount [
        char := aChar.
        count := aCount.
    ]

    <= other [
        ^ (count < other count)
        | ((count = other count) & (char <= other char)) 
    ]

    printBase: aStream [
    |fixhl|
        ^ aStream << self class << '(' << char << ',' << count
    ]
    printOn: aStream [
        (self printBase: aStream) << ')'.
    ]

    inorder: visitor prefix: string [
        visitor value: char value: string.
    ]

    binTreeDump: aBitstream [
        aBitstream putBit: 0.
        (1 to: 8) do: [:index | 
            aBitstream putBit: ( (self char asInteger) bitAt: index) ].
        char asInteger = 0 ifTrue: [
            aBitstream putBit: 0.
        ].
        char asInteger = 256 ifTrue: [
            aBitstream putBit: 1.
        ]
    ]
]

" Tree "
Leaf subclass: Tree [
    |left right|

    Tree class >> new: aChar count: aCount left: aLeft right: aRight [
        |result|
        result := super new: aChar count: aCount.
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

    printOn: aStream [
        (self printBase: aStream) << ',' << left << ',' << right << ')'.
    ]

    inorder: visitor prefix: string [
        left inorder: visitor prefix: string, '0'.
        right inorder: visitor prefix: string, '1'.
    ]

    binTreeDump: aBitstream [
        left binTreeDump: aBitstream.
        right binTreeDump: aBitstream.
        aBitstream putBit: 1.
    ]
]

nl := Character nl.
dFlag := false.
cFlag := false.
tFlag := false.
uFlag := false.
infile := ''.
outfile := stdout.

(1 to: Smalltalk arguments size)
do: [:index |
    argu := (Smalltalk arguments at: index).
    (argu includes: $-) ifTrue: [
        (argu includes: $d) ifTrue: [dFlag := true.].
        (argu includes: $c) ifTrue: [cFlag := true.].
        (argu includes: $t) ifTrue: [tFlag := true.].
        (argu includes: $u) ifTrue: [uFlag := true.].
    ] ifFalse: [
        (infile isEmpty) ifTrue: [
            infile := argu.
        ] ifFalse: [
            outfile :=  argu.
        ]
    ]
].

" Debug Flag "
dFlag ifTrue: [
    'Debug turned on' display.
    nl display.
    tFlag ifTrue: [
        'Decoding tree will be printed' display.
        nl display.
    ]. 
    cFlag ifTrue: [
        'Compressing: ' display.
        infile display.
        (outfile isEmpty) ifFalse: [
            ' to file ' display.
            outfile display.
            nl display.
        ] ifTrue: [
            ' to stdout' display.
            nl display.
        ]
    ].
    uFlag ifTrue: [
        'Uncompressing: ' display.
        infile display.
        (outfile isEmpty) ifFalse: [
            ' to file ' display.
            outfile display.
            nl display.
        ] ifTrue: [
            ' to stdout' display.
            nl display.
        ]
    ].    
].

" Read File and Write File Streams "
|readfile writefile|
readfile := FileStream open: infile 
                       mode: FileStream read.
writefile := FileStream open: outfile
                        mode: FileStream write.

" Frequency Table "
|frequencyTable|
uFlag ifFalse: [frequencyTable := readfile parseFile.].
dFlag ifTrue: [
    (0 to: frequencyTable size - 1)
    do: [:index |
        index display.
        ' ' display.
        item := frequencyTable at: index.
        item display.
        nl display.
    ].
].
|pq|
pq := SortedCollection new.
" Here we build the pq. "
uFlag ifFalse:[
    (0 to: frequencyTable size - 1)
    do: [:index |
        |aLeaf aCount aChar|
        aCount := frequencyTable at: index.
        aCount isInteger ifTrue: [
            aChar := index asCharacter.
            aLeaf := Leaf new: aChar  count: aCount.
            pq add: aLeaf.
        ].
    ].
].
dFlag ifTrue: [pq inspect.].

" Here we build a tree from the pq. "
uFlag ifFalse: [
    [pq size > 1] whileTrue: [
        |last seclast aTree treeChar treeCount|
        last := pq removeFirst.
        seclast := pq removeFirst.
        treeChar := last char.
        treeCount := last count + seclast count.
        aTree := Tree new: treeChar count: treeCount left: last 
                                                     right: seclast.
        pq add: aTree.
    ].
].
dFlag ifTrue: [pq inspect.].
|theTree encodeTable|
uFlag ifFalse: [ theTree := pq removeLast. ].
encodeTable := ZeroArray new: 257.
uFlag ifFalse: [
    theTree inorder: [:char :string |
        |index count|
        index := char asInteger.
        encodeTable at: index put: string.
    ] prefix: ''.
].
tFlag ifTrue: [ "Dump to stdout"
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
                stdout << char << ' ' << count << ' ' << string << nl.
            ] ifFalse: [
                stdout << index << ' ' << count << ' ' << string << nl.
            ].
        ].
    ]. 
    readfile close.
].
cFlag ifTrue: [ "Perform compression."
    |encString|
    aBitstream := BitStream new: writefile.
    readfile reset.
    theTree binTreeDump: aBitstream.
    aBitstream putBit: 1. "This will pop the last thing off the stack when we decode."
    [readfile atEnd not] whileTrue: [
        |index|
        index := readfile next asInteger.
        encString := (encodeTable at: index).
        aBitstream putString: encString.
    ].
    encString := encodeTable at: 256.
    aBitstream putString: encString.
    aBitstream padOut.
    readfile close.
    writefile close.
].

uFlag ifTrue: [ "Perform decompression."
    outBitstream := BitStream new: readfile.
    inBitstream := BitStream new: writefile.
   
    stack := OrderedCollection new.
    treeComplete := false.
    decodeTree := nil.

    [(outBitstream atEnd not) & (treeComplete = false)] whileTrue: [
        |ordChar|
        ordChar := outBitstream nextBit.
        ordChar = 0 ifTrue: [
            realChar := 0.
            basetwo := 1.

            (1 to: 8) do: [:skip |
                |currbit|
                currbit := outBitstream nextBit.
                (currbit = 1)
                ifTrue: [realChar := realChar + basetwo.].
                basetwo := basetwo *2.
            ].
            realChar = 0 ifTrue: [
                |currbit|
                currbit := outBitstream nextBit.
                currbit =1 ifTrue: [stack addFirst: (Leaf new: 256 asCharacter count: 1).]
                           ifFalse: [stack addFirst: (Leaf new: 0 asCharacter count: 1).].
            ] ifFalse: [stack addFirst: (Leaf new: realChar asCharacter count: 1).].
        ] ifFalse: [
            stack size > 0 ifTrue: [
                right := stack removeFirst.
                stack size = 0 ifTrue: [
                    decodeTree := right.
                    treeComplete := true.
                ] ifFalse: [
                    left := stack removeFirst.
                    stack addFirst: (Tree new: (left char) count: 1 left: left right: right).
                ].
            ].
        ].
    ].
    dFlag ifTrue: [decodeTree inspect.].

    tempTree := decodeTree.
    eof := false.
    [(outBitstream atEnd not) & (eof = false)] whileTrue: [
        |enbit|

        enbit := outBitstream nextBit.

        enbit = 0 ifTrue: [
            tempTree  isLeaf ifTrue: [
                tempTree char asInteger = 256 ifTrue: [eof = true.].
            ] ifFalse: [
                tempTree := tempTree left.
                tempTree  isLeaf ifTrue: [
                    tempTree char asInteger = 256 ifTrue: [ eof = true.]
                    ifFalse: [
                        writefile << tempTree char.
                    ].
                    tempTree := decodeTree.
                ].
            ].
        ] ifFalse: [
            tempTree isLeaf ifTrue: [
                tempTree char asInteger = 256 ifTrue: [eof := true.].
            ] ifFalse: [
                tempTree = tempTree right.
                tempTree  isLeaf ifTrue: [
                    tempTree char asInteger = 256 ifTrue: [ eof := true.]
                    ifFalse: [
                        writefile << tempTree char.
                    ].
                    tempTree := decodeTree.
                ].
            ].
        ].
    ].

    readfile close.
    writefile close.
].
