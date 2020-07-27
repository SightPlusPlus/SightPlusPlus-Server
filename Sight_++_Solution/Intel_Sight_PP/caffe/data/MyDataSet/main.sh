# !/bin/bash

python create_data.py

echo "Finished generating text files in ImageSets/Main!"

bash create_list.sh

echo "Finished generating trainval.txt, test.txt and test_name_size.txt!"

bash create_data.sh

echo "Finished generating the LMDB in Dataset directory!"

cur_dir=$(cd $( dirname ${BASH_SOURCE[0]} ) && pwd )
data_dir="$(dirname "$cur_dir")"
cafe_dir="$(dirname "$data_dir")"

cd $cafe_dir/examples/MobileNet-SSD

bash gen_model.sh $1

echo "Finished generating your own training prototxt!"

rm -rf trainval_lmdb
rm -rf test_lmdb

ln -s $cur_dir/Main/lmdb/Main_trainval_lmdb trainval_lmdb
ln -s $cur_dir/Main/lmdb/Main_test_lmdb test_lmdb

echo "Finished creating symlinks to the LMDB in Dataset directory!"

rm -rf snapshot

bash train.sh

echo "Finished training...!"

python merge_bn.py --model ./example/MobileNetSSD_deploy.prototxt --weights ./snapshot/*.caffemodel

echo "Finished obtaining the caffemodel and prototxt based on latest snapshots generated!"

root_dir="$(dirname "$cafe_dir")"

cp no_bn.caffemodel $root_dir/models
cp no_bn.prototxt $root_dir/models

echo "Finished moving the caffemodel and prototxt to the models direcoty!"
echo "Please rename the caffemodel and prototxt in models directory and write a corresponding classnames text file for the model!"

