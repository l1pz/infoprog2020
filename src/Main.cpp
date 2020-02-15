#include "pch.h"
#include "LatnivaloKezelo.hpp"

using namespace LatnivaloKezelo;

int main()
{
	Betolt("latnivalok.csv");
	IO::Listaz(latnivalok);
	auto a = KeresKategoria("természeti kincsek");
	if (a.has_value())
		for (auto latnivalo : a.value())
			Torol(latnivalo);
	else
		std::cout << "Nincs ilyen!\n";
	std::cout << std::endl;
	IO::Listaz(latnivalok);
}