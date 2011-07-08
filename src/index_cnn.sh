#!/bin/bash

for f in `ls ../canned_data/cnn`; do echo $f; ../bin/Tools/Indexer "$f" "`cat ../canned_data/cnn/$f | sed -e's/\"/ /g'`"; done;
