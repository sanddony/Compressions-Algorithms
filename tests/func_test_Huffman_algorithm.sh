#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0

DELETE_MODE=$1

BUILD_DIR="../build"
DATA_SAMPLES_DIR="../data-samples"
RESULTS_DIR="../data-samples/results"
ALGORITHM="-a huff"

declare -a tests=(
  "$DATA_SAMPLES_DIR/eng_text.txt"
  "$DATA_SAMPLES_DIR/eng_text_2.txt"
  "$DATA_SAMPLES_DIR/rus_text.txt"
  "$DATA_SAMPLES_DIR/rus_text_2.txt"
  "$DATA_SAMPLES_DIR/test.txt"
  "$DATA_SAMPLES_DIR/tmp.txt"
  "$DATA_SAMPLES_DIR/large_text_eng.txt"
  "$DATA_SAMPLES_DIR/large_text_ru.txt"
  "$BUILD_DIR/compress"
)

testing() {
  extern="$(echo $@ | grep -e "\.[A-z]*$" -o)"
  echo $extern
  if [ "$extern" ]; then
    encode_file=$(echo $@ | sed "s/$extern/_encode$extern/")
    decode_file=$(echo $@ | sed "s/$extern/_decode$extern/")
  else
    file_name="$(echo $@ | grep -e "[A-z]*$" -o)"
    encode_file="$DATA_SAMPLES_DIR/$file_name.encode"
    decode_file="$DATA_SAMPLES_DIR/$file_name.decode"
  fi
  
  START_ENCODE=$(date +%s%N | sed "s/N//")
  $BUILD_DIR/compress $ALGORITHM -e -i $@ -o $encode_file
  echo "$BUILD_DIR/compress $ALGORITHM -e -i $@ -o $encode_file"
  END_ENCODE=$(date +%s%N | sed "s/N//")
  ENCODE_TIME=$(($END_ENCODE - $START_ENCODE))


  START_DECODE=$(date +%s%N | sed "s/N//")
  $BUILD_DIR/compress $ALGORITHM -d -i $encode_file -o $decode_file
  echo "$BUILD_DIR/compress $ALGORITHM -d -i $encode_file -o $decode_file"
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
  ../tests/print_compression_infographic $ENCODE_TIME $encode_file $DECODE_TIME $decode_file

}


gcc ../tests/print_compression_infographic.c -o ../tests/print_compression_infographic

for i in "${tests[@]}"; do
    var="-$var1"
    testing $i
done


echo "\033[31mFAIL: $FAIL\033[0m"
echo "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"

if [ "$DELETE_MODE" == "1" ]; then
    rm -f $DATA_SAMPLES_DIR/*_encode*
    rm -f $DATA_SAMPLES_DIR/*_encode
    rm -f $DATA_SAMPLES_DIR/*_decode*
    rm -f $DATA_SAMPLES_DIR/*_decode
    rm -f ../tests/print_compression_infographic
fi