" This Source Code Form is subject to the terms of the Mozilla Public
" License, v. 2.0. If a copy of the MPL was not distributed with this
" file, You can obtain one at https://mozilla.org/MPL/2.0/.
"
" Language: Jawa
" Maintainer: Peter Grajcar

if exists("b:current_syntax")
    finish
endif

syn keyword modifiers       prywatny prywatny                               skipwhite
syn keyword modifiers       chroniony chroniona                             skipwhite
syn keyword modifiers       publiczny publiczna                             skipwhite
syn keyword modifiers       statyczny statyczna                             skipwhite
syn keyword modifiers       końcowy końcowa                                 skipwhite
syn keyword modifiers       ojczysty ojczysta                               skipwhite
syn keyword modifiers       zmeinny zmeinna                                 skipwhite
syn keyword modifiers       zsynchronizowany zsynchronizowana               skipwhite
syn keyword modifiers       abstrakcyjny abstrakcyjna                       skipwhite
syn keyword modifiers       rzejściowy rzejściowa                           skipwhite
syn keyword modifiers       ścisłezp ścisłezp                               skipwhite
syn keyword controlFlow     jeżeli albo dla wykonaj dopóki zwróć spróbuj    skipwhite
syn keyword controlFlow     łap wreszcie kontyntynuj przełącz domyślna złam skipwhite
syn keyword controlFlow     przypad rzuć                                    skipwhite
syn keyword types           void całość krótki znak bajt                    skipwhite, nextgroup=idf
syn keyword types           długy podwójny pojedynczy boolowski             skipwhite, nextgroup=idf
syn keyword classes         klasa międzymordzie @międzymordzie wyliczenie   skipwhite, nextgroup=classIdf
syn keyword keywords        nadzbiór przedłuża                              skipwhite
syn keyword keywords        realizuje to potwierdzić pakiet zaimportuj      skipwhite
syn keyword keywords        rzuca wystąpienie nowy                          skipwhite
syn keyword reserved        kurwa                                           skipwhite

syn keyword boolLiteral     prawda nieprawda    skipwhite

syn match numberLiteral     '\d\+'              display
syn match numberLiteral     '0[XxBb]\d\+'       display
syn match numberLiteral     '\d\+'              display
syn match numberLiteral     '[-+]\d\+'          display

syn match annotation        '@\([^[:punct:][:space:]]\|[_$]\)\+' display
syn match idf               '\([^[:punct:][:space:]]\|[_$]\)\+' display
syn match classIdf          '\([^[:punct:][:space:]]\|[_$]\)\+' display

syn match operators         '[;\+\*\-&\^\|\?\.:%~\!=<>/]'       display

syn match comment           '//.*$' display

syn region stringLiteral    start='"'   end='"'
syn region charLiteral      start='\''  end='\''
syn region commentMultiline start='/\*' end='\*/' fold

syn keyword todo TODO FIXME

let b:current_syntax = "jawa"

hi def link todo                    Todo
hi def link keywords                Keyword
hi def link controlFlow             Keyword
hi def link types                   Type
hi def link modifiers               StorageClass
hi def link classes                 Structure
hi def link operators               Operator
hi def link boolLiteral             Keyword
hi def link stringLiteral           Constant
hi def link charLiteral             Constant
hi def link numberLiteral           Constant
hi def link idf                     Identifier
hi def link classIdf                Identifier
hi def link comment                 Comment
hi def link commentMultiline        Comment
hi def link annotation              Special
hi def link reserved                Special