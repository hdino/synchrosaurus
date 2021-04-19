#!/usr/bin/env bash

BASE_DIR="test_structure"
if [ -d "$BASE_DIR" ]; then
  echo "${BASE_DIR} already exists. Aborting."
  exit -1
fi

mkdir -p "${BASE_DIR}/d1/d1_d1/d1_d1_d1"
mkdir -p "${BASE_DIR}/d1/d1_d1/d1_d1_d2"
mkdir -p "${BASE_DIR}/d1/d1_d2"
mkdir -p "${BASE_DIR}/d2/d2_d1"
mkdir -p "${BASE_DIR}/d3"
mkdir -p "${BASE_DIR}/d4"
mkdir -p "${BASE_DIR}/d5"

chmod 700 "${BASE_DIR}/d5"
sudo chown root:root "${BASE_DIR}/d5"

cp test_files/Document* "${BASE_DIR}/d4"
cp test_files/*.txt "${BASE_DIR}/d4"
cp test_files/Empty* "${BASE_DIR}/d2/d2_d1"
cp test_files/Painting* "${BASE_DIR}/d2/d2_d1"
cp test_files/HelloWorld* "${BASE_DIR}/d1/d1_d1/d1_d1_d2"

ln -s "../../../d4/MyText.txt" "${BASE_DIR}/d1/d1_d1/d1_d1_d2/RelLinkToMyText.txt"
ln -s "${PWD}/${BASE_DIR}/d4/MyText.txt" "${BASE_DIR}/d1/d1_d1/d1_d1_d2/AbsLinkToMyText.txt"
ln -s "d1" "${BASE_DIR}/link_to_d1"
