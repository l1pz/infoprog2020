#include "pch.h"
#include "LatnivaloKezelo.hpp"

using namespace LatnivaloKezelo;

int main()
{
	Beolvas("latnivalok.csv");
	IO::Listaz(latnivalok);
	auto a = KeresKategoria("természeti kincsek");
	if (a.has_value())
		for (auto latnivalo : a.value())
			std::cout << latnivalo->nev << ", ";
	else
		std::cout << "Nincs ilyen!\n";
}