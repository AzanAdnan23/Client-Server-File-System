#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")

struct Node
{
public:
	int id;
	Node *left;
	Node *right;
	string filename;
};
Node *insert(Node *root, int data, string fn)
{
	if (root == NULL)
	{
		Node *temp = new Node();
		temp->id = data;
		temp->left = NULL;
		temp->right = NULL;
		temp->filename = fn;

		return temp;
	}
	else if (root->id == 0)
	{
		Node *temp;
		root->id = data;
		return root;
	}
	else
	{
		Node *temp = new Node();
		temp->id = data;
		temp->left = NULL;
		temp->right = NULL;
		temp->filename = fn;

		if (data <= root->id)
		{
			temp = insert(root->left, data, fn);
			root->left = temp;
		}
		else
		{
			temp = insert(root->right, data, fn);
			root->right = temp;
		}
		return root;
	}
}
Node *minnode(Node *root)
{
	Node *temp = root;
	while (temp && temp->left != 0)
	{
		temp = temp->left;
	}
	return temp;
}
Node *remove(Node *root, int id)
{
	if (root == NULL)
	{
		return root;
	}
	if (id > root->id)
	{
		root->right = remove(root->right, id);
	}
	else if (id < root->id)
	{
		root->left = remove(root->left, id);
	}
	else
	{
		if (root->left == NULL && root->right == NULL)
		{
			return 0;
		}
		else if (root->left == NULL)
		{
			Node *temp = root->right;
			delete root;
			return temp;
		}
		else if (root->left == NULL)
		{
			Node *temp = root->left;
			delete root;
			return temp;
		}
		Node *temp = minnode(root->right);
		root->id = temp->id;
		root->right = remove(root->right, temp->id);
	}
	return root;
}
int Search(Node *root, int data)
{
	while (root != NULL)
	{
		if (root->id == data)
		{
			// cout << endl << " Number found  " << endl;
			return 1;
		}
		else if (root->id > data)
		{
			root = root->left;
		}
		else
		{
			root = root->right;
		}
	}
	// cout << endl << " Number not found " << endl;
	return 0;
}
void Display(Node *root)
{
	if (root != NULL)
	{
		cout << root->filename << " " << root->id << endl;
		Display(root->left);
		Display(root->right);
	}
}
class Hash
{
private:
	Node *tree;

public:
	Hash()
	{
		tree = new Node[10];
		for (int i = 0; i < 10; i++)
		{
			tree[i].id = 0;
			tree[i].left = NULL;
			tree[i].right = NULL;
		}
	}
	int hashfunction(int id)
	{
		return id % 10;
	}
	void Insert(int id, string filname)
	{
		int index = hashfunction(id);

		insert(&tree[index], id, filname);
	}
	void Delete(int id)
	{
		int index = hashfunction(id);
		if (Search(&tree[index], id) == 1)
		{
			remove(&tree[index], id);
		}
	}
	int search(int id)
	{
		int index = hashfunction(id);
		return Search(&tree[index], id);
	}
	void display(int id)
	{
		int index = hashfunction(id);
		Display(&tree[index]);
	}
	string getfilename(int id)
	{
	}
};
int main()
{
	Hash obj;

	WSADATA wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	getaddrinfo("10.62.52.27", "543210", &hints, &result);

	SOCKET listensocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	bind(listensocket, result->ai_addr, (int)result->ai_addrlen);

	listen(listensocket, SOMAXCONN);

	SOCKET clientsocket = accept(listensocket, NULL, NULL);

	char buff[500];
	int buflen = 500;

	while (true)
	{
		recv(clientsocket, buff, buflen, 0);

		if (buff[0] == '4')
		{
			cout << " Exit sucessfully " << endl;
			break;
		}
		int id = 0, count = 0, temp = 0, next = 0;
		temp = buff[1] - 48;

		for (int i = 1;; i++)
		{
			if (buff[i] == '=')
			{
				break;
			}
			count++;
		}
		for (int i = 1; i < count; i++)
		{
			next = temp * 10 + buff[i + 1] - 48;
			temp = next;
		}
		id = temp;
		// cout << " ID " << id << endl;

		string filenames;

		if (buff[0] == '1')
		{
			// insert

			filenames = "filenNo" + to_string(id) + ".txt";

			obj.Insert(id, filenames);
			// obj.display(id);

			//'�';
			int count99 = 0;
			for (int i = 0;; i++)
			{
				count99++;
				if (buff[i] == '�')
				{
					break;
				}
			}
			int i = count99;
			ofstream File(filenames);
			while (true)
			{

				File << buff[i];
				// cout << buff[i];
				if (buff[i] == '�')
				{
					break;
				}
				i++;
			}

			File.close();
		}
		else if (buff[0] == '2')
		// Delete
		{
			obj.Delete(id);
			char *temp;
			string obj(filenames);
			temp = &obj[0];

			remove(temp);
		}

		else if (buff[0] == '3')
		{
			// Search
			int check;
			char buffa[50];
			check = obj.search(id);
			if (check == 1)
			{
				// cout << "Number found";
				buffa[0] = '1';
				send(clientsocket, buffa, 50, 0);
			}
			else
			{
				// cout << "Not found ";

				buffa[0] = '0';
				send(clientsocket, buffa, 50, 0);
			}
		}
	}

	//	cout << " File data: " << endl;

	shutdown(clientsocket, SD_SEND);
	closesocket(clientsocket);
}