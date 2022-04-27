#include "list2.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>

void add(list2& list, int data)
{
	elem* newel = new elem;
	newel->x = data;
	list.count++;
	if (!list.first)
	{
		list.first = list.last = newel;
	}

	else
	{
		newel->prev = list.last;
		list.last->next = newel;
		list.last = newel;
	}
}

bool remove(list2& list, int pos)
{
	if (pos < 0 || pos >= list.count)
	{
		return false;
	}
	list.count--;
	if (pos == 0)
	{
		elem* rem = list.first;
		list.first = list.first->next;
		if (list.first) list.first->prev = nullptr;
		delete rem;
		return true;
	}
	if (pos == list.count)
	{
		elem* rem = list.last;
		list.last = list.last->prev;
		list.last->next = nullptr;
		delete rem;
		return true;
	}
	bool flw = (pos <= list.count / 2);
	int p = 1;
	elem* curr;
	if (flw)
	{
		curr = list.last->next;
	}
	else
	{
		curr = list.last->prev;
		pos = list.count - pos;
	}
	while (p < pos)
	{
		curr = flw ? curr->next : curr->prev;
		p++;
	}
	curr->prev->next = curr->next;
	curr->next->prev = curr->prev;
	delete curr;
	return true;

}

void clear(list2& list)
{
	elem* rem;
	while (list.first)
	{
		rem = list.first;
		list.first = list.first->next;
		delete rem;
	}
	list.last = nullptr;
	list.count = 0;
}
const int* get(list2 list, int pos)
{
	if (!list.first && pos < 0 || pos >= list.count) return nullptr;
	if (!pos) return &list.first->x;
	if (pos == list.count - 1) return &list.last->x;
	int p = 1;
	bool fwd = pos <= list.count / 2;
	elem* curr;
	if (fwd) curr = list.first->next;
	else
	{
		curr = list.last->prev;
		pos = list.count - pos - 1;
	}
	while (curr && p < pos)
	{
		p++;
		curr = fwd ? curr->next : curr->prev;
	}
	return &curr->x;
}
//int count(elem* first)
//{
//	int temp = 0;
//	elem* curr = first;
//	while (curr) {
//		temp++;
//		curr = curr.
//	}
//	return temp;
//}
bool is_get()
{
	size_t realsize;
	char* value = new char[5];
	getenv_s(&realsize, value, 5, "REQUEST_METHOD");
	bool res = !_strcmpi(value, "GET");
	delete[] value;
	return res;
}

/**
 * Получение информации о длине сообщения от пользователя, переданного
 * методом POST
 */
size_t get_content_length()
{
	size_t realsize;
	char* value = new char[11];
	getenv_s(&realsize, value, 11, "CONTENT_LENGTH");
	size_t size;
	if (!realsize) size = 0;
	else sscanf_s(value, "%d", &size);
	delete[] value;
	return size;
}

/**
 * Получение данных из формы в необработанном виде
 */
void get_form_data(char*& data)
{
	delete[] data;
	if (is_get())
	{
		size_t realsize;
		char* value = new char[65536];
		getenv_s(&realsize, value, 65536, "QUERY_STRING");
		data = new char[realsize + 1];
		strcpy_s(data, realsize + 1, value);
		delete[] value;
	}
	else
	{
		size_t buf_size = get_content_length();
		data = new char[buf_size + 1];
		fread_s(
			data,
			buf_size + 1,
			sizeof(char),
			buf_size,
			stdin
		);
		data[buf_size] = 0;
	}
}

/**
 * Получение значения конкретного параметра, полученного от пользователя
 */
void get_param_value(char*& value, const char* param_name, const char* data)
{
	delete[] value;
	value = nullptr;
	char* str = _strdup(data);
	char* tmp = str;
	char* cont;
	while (char* part = strtok_s(tmp, "&", &cont))
	{
		tmp = nullptr;
		char* val;
		char* key = strtok_s(part, "=", &val);
		if (!_strcmpi(param_name, key))
		{
			str_decode(value, val);
			//value = _strdup(val);
			delete[] str;
			return;
		}
	}
	delete[] str;
	value = new char[1];
	value[0] = 0;
}

/**
 * Раскодирование переданных данных
 */
void str_decode(char*& dec_str, const char* enc_str)
{
	char* res = new char[strlen(enc_str) + 1];
	int i = 0, j = 0;
	while (enc_str[i])
	{
		if (enc_str[i] == '+')
		{
			res[j] = ' ';
		}
		else
		{
			if (enc_str[i] == '%')
			{
				char ch[3] = { enc_str[i + 1], enc_str[i + 2], 0 };
				int c;
				sscanf_s(ch, "%X", &c);
				res[j] = c;
				i += 2;
			}
			else
			{
				res[j] = enc_str[i];
			}
		}
		i++;
		j++;
	}
	res[j] = 0;
	size_t len = strlen(res) + 1;
	delete[] dec_str;
	dec_str = new char[len];
	strcpy_s(dec_str, len, res);
	delete[] res;
}
