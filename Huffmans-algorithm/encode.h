#ifndef COMPRESSIONS_ALGORITHMS_ENCODE_H
#define COMPRESSIONS_ALGORITHMS_ENCODE_H

int Encode(files files);
node** SortRoots(node** nodes_list, int list_size);
node** GetFrequencyOfBytes(files files, int* sym_count);
node* BuildTree(node* nodes_list, int sym_count);
int SerializationOfTheTree(files files, node* root);
// node* BinarySearch(node* root, char byte);
int WriteEncodeFile(files files, node* root);
node* CopyNode(node input_node);

#endif //COMPRESSIONS_ALGORITHMS_ENCODE_H
