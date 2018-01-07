#include "Archiver.h"

Archiver::Archiver()
{
	this->path_from.clear();
	this->path_to.clear();
	this->cnt_byte = 0;
	this->ready = false;
}
Archiver::~Archiver()
{
	this->path_from.clear();
	this->path_to.clear();
	this->cnt_byte = 0;
	this->ready = false;
}

bool Archiver::Ready()
{
	return this->ready;
}
void Archiver::Initialize(string from, string to, size_t cnt)
{
	if (from.empty() || to.empty()) return;
	fstream f(from);
	if (!f.is_open()) return;
	f.close();
	f.open(to);
	if (f.is_open()) { f.close(); return; }
	this->path_from = from;
	this->path_to = to;
	this->cnt_byte = cnt;
	this->ready = true;
}
void Archiver::Run()
{
	if (!this->ready) return;


}
