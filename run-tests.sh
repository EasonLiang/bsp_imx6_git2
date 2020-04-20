#!/bin/bash
# A simple testframe runner for the bzip2 test suite.
#
# Copyright (C) 2019, Mark J. Wielaard  <mark@klomp.org>
#
# This file is free software.  You can redistribute it and/or modify
# it under the terms of the GNU General Public License (GPL); either
# version 3, or (at your option) any later version.

VALGRIND="valgrind"
VALGRIND_ARGS="-q --error-exitcode=9"
BZIP2="bzip2"
TESTS_DIR="."
IGNORE_MD5=0

for i in "$@"
do
case $i in
    --bzip2=*)
    BZIP2="${i#*=}"
    shift
    ;;
    --without-valgrind)
    VALGRIND=""
    VALGRIND_ARGS=""
    shift
    ;;
    --ignore-md5)
    IGNORE_MD5=1
    shift
    ;;
    --tests-dir=*)
    TESTS_DIR="${i#*=}"
    ;;
    *)
    echo "run-tests [--bzip2=bzip2-command] [--without-valgrind]"
    echo "          [--ignore-md5] [--tests-dir=/path/to/bzip2-tests-dir]"
    exit 1
    ;;
esac
done

if ! type "valgrind" > /dev/null 2>&1; then
  VALGRIND=""
  VALGRIND_ARGS=""
fi

echo "Testing ${BZIP2} in directory ${TESTS_DIR}"
if [ "$VALGRIND" != "" ]; then
  echo "Using valgrind: Yes"
else
  echo "Using valgrind: No"
fi
if [[ ${IGNORE_MD5} -eq 0 ]]; then
  echo "Checking md5 sums: Yes"
else
  echo "Checking md5 sums: No"
fi

# Remove any left over tesfilecopies from previous runs first.
find ${TESTS_DIR} -type f -name \*\.testfilecopy\.bz2 -exec rm \{\} \;

# First test the good bz2 files.
echo "Testing decompression and recompression..."
echo

badtests=()

nogood=0
while IFS= read -r -d '' bzfile; do
  file="${bzfile%.*}"
  copy="${file}.testfilecopy"
  bzcopy="${copy}.bz2"
  md5file="${file}.md5"

  # Decompress it.
  rm -f "${file}"
  ${VALGRIND} ${VALGRIND_ARGS} ${BZIP2} -k -d -q ${bzfile} \
    && { echo "PASS: ${bzfile} Decompress"; } \
    || { echo "FAIL: ${bzfile} Decompress";
         badtests=("${badtests[@]}" $"${bzfile} bad decompress result")
         nogood=$[${nogood}+1]; continue; }

  if [[ ${IGNORE_MD5} -ne 1 ]]; then
    md5sum -c ${md5file} < ${file} > /dev/null \
      && { echo "PASS: ${bzfile} md5sum Matched"; } \
      || { echo "FAIL: ${bzfile} md5sum Matched";
           badtests=("${badtests[@]}" $"${file} md5sum doesn't match")
           nogood=$[${nogood}+1]; continue; }
  fi

  # Compress and decompress a copy
  mv "${file}" "${copy}"
  rm -f "${bzcopy}"
  ${VALGRIND} ${VALGRIND_ARGS} ${BZIP2} -z -q -s ${copy} \
    && { echo "PASS: ${bzfile} Recompress "; } \
    || { echo "FAIL: ${bzfile} Recompress";
         badtests=("${badtests[@]}" $"${copy} bad result")
         nogood=$[${nogood}+1]; continue; }
  ${VALGRIND} ${VALGRIND_ARGS} ${BZIP2} -d -q -s ${bzcopy} \
    && { echo "PASS: ${bzfile} Redecompress"; } \
    || { echo "FAIL: ${bzfile} Redecompress";
         badtests=("${badtests[@]}" $"${bzcopy} bad result")
         nogood=$[${nogood}+1]; continue; }

  if [[ ${IGNORE_MD5} -ne 1 ]]; then
    md5sum -c ${md5file} < ${copy} > /dev/null \
      && { echo "PASS: ${bzfile} md5sum ReMatched"; } \
      || { echo "FAIL: ${bzfile} md5sum ReMatched";
           badtests=("${badtests[@]}" $"${copy} md5sum doesn't match")
           nogood=$[${nogood}+1]; continue; }
  fi

  rm "${copy}"

  # Now do it all again in "small" mode.
  rm -f "${file}"
  ${VALGRIND} ${VALGRIND_ARGS} ${BZIP2} -k -d -q -s ${bzfile} \
    && { echo "PASS: ${bzfile} Decompress (small)"; } \
    || { echo "FAIL: ${bzfile} Decompress (small)";
         badtests=("${badtests[@]}" $"${bzfile} bad decompress result")
         nogood=$[${nogood}+1]; continue; }

  if [[ ${IGNORE_MD5} -ne 1 ]]; then
    md5sum -c ${md5file} < ${file} > /dev/null \
      && { echo "PASS: ${bzfile} Md5sum Matched (small)"; } \
      || { echo "FAIL: ${bzfile} Md5sum Matched (small)";
           badtests=("${badtests[@]}" $"${file} md5sum doesn't match")
           nogood=$[${nogood}+1]; continue; }
  fi

  # Compress and decompress a copy
  mv "${file}" "${copy}"
  rm -f "${bzcopy}"
  ${VALGRIND} ${VALGRIND_ARGS} ${BZIP2} -z -q -s ${copy} \
    && { echo "PASS: ${bzfile} Recompress (small)"; } \
    || { echo "FAIL: ${bzfile} Recompress (small)";
         badtests=("${badtests[@]}" $"${copy} bad result")
         nogood=$[${nogood}+1]; continue; }
  ${VALGRIND} ${VALGRIND_ARGS} ${BZIP2} -d -q -s ${bzcopy} \
    && { echo "PASS: ${bzfile} Redecompress (small)"; } \
    || { echo "FAIL: ${bzfile} Redecompress (small)";
         badtests=("${badtests[@]}" $"${bzcopy} bad result")
         nogood=$[${nogood}+1]; continue; }

  if [[ ${IGNORE_MD5} -ne 1 ]]; then
    md5sum -c ${md5file} < ${copy} > /dev/null \
      && { echo "PASS: ${bzfile} md5sum ReMatched (small)"; } \
      || { echo "FAIL: ${bzfile} md5sum ReMatched (small)";
           badtests=("${badtests[@]}" $"${copy} md5sum doesn't match")
           nogood=$[${nogood}+1]; continue; }
  fi

  rm "${copy}"

done < <(find ${TESTS_DIR} -type f -name \*\.bz2 -print0)

echo
if [[ ${nogood} -eq 0 ]]; then
  echo "All .bz files decompressed and recompressed correctly."
else
  echo "!!! ${nogood} .bz files did not decompressed/recompressed correctly."
fi
echo

# Then test all bad inputs.
echo "Testing detection of bad input data..."
echo

nobad=0
badbad=0
while IFS= read -r -d '' badfile; do

  ${VALGRIND} ${VALGRIND_ARGS} ${BZIP2} -k -d -q ${badfile} > /dev/null 2>&1
  ret=$?

  if [[ ${ret} -eq 0 ]]; then
    echo "FAIL: badness not detected in ${badfile}"
    nobad=$[${nobad}+1]
    badtests=("${badtests[@]}" $"${badfile} badness not detected")
    continue
  fi

  # Assumes "normal" badness is detected by exit code 1 or 2.
  # A crash or valgrind issue will be reported with something else.
  if [[ ${ret} != 1 ]] && [[ ${ret} != 2 ]]; then
    echo "FAIL: baddness caused baddness in ${BZIP2} for ${badfile}"
    badbad=$[${badbad}+1]
    badtests=("${badtests[@]}" $"${badfile} badness caused baddness")
    continue
  else
    echo "PASS: Correctly found data integrity errors in ${badfile}"
  fi

  ${VALGRIND} ${VALGRIND_ARGS} ${BZIP2} -k -d -q -s ${badfile} > /dev/null 2>&1
  ret=$?

  if [[ ${ret} -eq 0 ]]; then
    echo "FAIL: badness not detected in ${badfile} (small)"
    nobad=$[${nobad}+1]
    badtests=("${badtests[@]}" $"${badfile} badness not detected")
    continue
  fi

  # Assumes "normal" badness is detected by exit code 1 or 2.
  # A crash or valgrind issue will be reported with something else.
  if [[ ${ret} != 1 ]] && [[ ${ret} != 2 ]]; then
    echo "FAIL: baddness caused baddness in ${BZIP2} for ${badfile} (small)"
    badbad=$[${badbad}+1]
    badtests=("${badtests[@]}" $"${badfile} badness caused baddness")
    continue
  else
    echo "PASS: Correctly found data integrity errors in ${badfile} (small)"
  fi

done < <(find ${TESTS_DIR} -type f -name \*\.bz2.bad -print0)

badresults=$[${nobad}+${badbad}]
echo
if [[ ${badresults} -eq 0 ]]; then
  echo "Correctly found all bad file data integrity errors."
else
  if [[ ${nobad} -ne 0 ]]; then
    echo "!!! ${nobad} files not correctly detected as bad."
  fi
  if [[ ${badbad} -ne 0 ]]; then
    echo "!!! ${badbad} files caused baddness."
  fi
fi
echo

results=$[${nogood}+${badresults}]
if [[ ${results} -eq 0 ]]; then
  echo "All tests passed"
  exit 0
else
  echo "Bad results, look for FAIL and !!! in the logs above"
  printf ' - %s\n' "${badtests[@]}"
  exit 1
fi
