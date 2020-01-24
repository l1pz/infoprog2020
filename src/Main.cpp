#include "pch.h"
#include "LatnivaloKezelo.hpp"

using namespace LatnivaloKezelo;

int main()
{
	SetConsoleCP(1250);
	SetConsoleOutputCP(1250);
	std::setlocale(LC_ALL, "1250");
	Beolvas("latnivalok.csv");
	IO::Listaz(latnivalok);
	auto a = KeresAzonosito(25);
	if (a.has_value())
		std::cout << a.value()->nev;
	else std::cout << "Nincs ilyen!\n";
}