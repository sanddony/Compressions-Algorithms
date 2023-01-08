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
)

testing() {
  extern=$(echo $@ | grep -e "\.[A-z]*$" -o)
  echo $extern
  encode_file=$(echo $@ | sed "s/$extern/_encode$extern/")
  decode_file=$(echo $@ | sed "s/$extern/_decode$extern/")

  $BUILD_DIR/compress $ALGORITHM -e -i $@ -o $encode_file
  $BUILD_DIR/compress $ALGORITHM -d -i $encode_file -o $decode_file
  
  DIFF_RES="$(diff -s $@ $decode_file)"
  
  ((COUNTER++))
  if [ "$DIFF_RES" == "Files $@ and $decode_file are identical" ]; then
    ((SUCCESS++))
    echo -e "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m grep $@"
  else
    ((FAIL++))
    echo -e "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m grep $@"
  fi


}


for i in "${tests[@]}"; do
    var="-$var1"
    testing $i
done


echo -e "\033[31mFAIL: $FAIL\033[0m"
echo -e "\033[32mSUCCESS: $SUCCESS\033[0m"
echo -e "ALL: $COUNTER"

if [ "$DELETE_MODE" == "1" ]; then
    rm -f $DATA_SAMPLES_DIR/*_encode*
    rm -f $DATA_SAMPLES_DIR/*_decode*
fi