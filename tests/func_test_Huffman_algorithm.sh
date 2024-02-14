#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0

DELETE_MODE=$1

BUILD_DIR="../build"
DATA_SAMPLES_DIR="../data-samples"
TESTS_DIR="."
RESULTS_DIR="../data-samples/results"
ALGORITHM="-a huff"


tests=$(find $DATA_SAMPLES_DIR/*)

testing() {
  extern="$(echo $@ | grep -e "\.[A-z]*$" -o)"
  if [ "$extern" ]; then
    encode_file=$(echo $@ | sed "s/$extern/_encode$extern/")
    decode_file=$(echo $@ | sed "s/$extern/_decode$extern/")
  else
    file_name="$(echo $@ | grep -e "[A-z]*$" -o)"
    encode_file="$DATA_SAMPLES_DIR/$file_name.encode"
    decode_file="$DATA_SAMPLES_DIR/$file_name.decode"
  fi
  
  START_ENCODE=$(date +%s%N | sed "s/N//")
  echo "$BUILD_DIR/compress $ALGORITHM -e -i $@ -o $encode_file"
  $BUILD_DIR/compress $ALGORITHM -e -i $@ -o $encode_file
  END_ENCODE=$(date +%s%N | sed "s/N//")
  ENCODE_TIME=$(($END_ENCODE - $START_ENCODE))


  START_DECODE=$(date +%s%N | sed "s/N//")
  echo "$BUILD_DIR/compress $ALGORITHM -d -i $encode_file -o $decode_file"
  $BUILD_DIR/compress $ALGORITHM -d -i $encode_file -o $decode_file
  END_DECODE=$(date +%s%N | sed "s/N//")
  DECODE_TIME=$(($END_DECODE - $START_DECODE))

  DIFF_RES="$(diff -s $@ $decode_file)"
  
  ((COUNTER++))
  if [ "$DIFF_RES" == "Files $@ and $decode_file are identical" ]; then
    ((SUCCESS++))
    echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m $@"
  else
    ((FAIL++))
    echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m $@"
  fi
  $TESTS_DIR/print_compression_infographic $ENCODE_TIME $encode_file $DECODE_TIME $decode_file
  if [ "$DELETE_MODE" == "1" ]; then
    rm -f $decode_file
    rm -f $encode_file
  fi
}


gcc $TESTS_DIR/print_compression_infographic.c -o $TESTS_DIR/print_compression_infographic

for i in $tests; do
    testing $i
done


echo "\033[31mFAIL: $FAIL\033[0m"
echo "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"

if [ "$DELETE_MODE" == "1" ]; then
    rm -f $TESTS_DIR/print_compression_infographic
fi
