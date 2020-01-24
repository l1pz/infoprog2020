#include "pch.h"
#include "Latnivalo.h"

Latnivalo::Latnivalo(const unsigned azon_, std::string_view nev_, const float hossz_, const float szel_, std::string_view kat_, const float atlIdo_, const std::string_view leiras_)
	: azon(azon_), nev(nev_), hossz(hossz_), kat(kat_), atlIdo(atlIdo_), leiras(leiras_)
{
	std::replace(nev.begin(), nev.end(), '_', ' ');
	std::replace(kat.begin(), kat.end(), '_', ' ');
	std::replace(leiras.begin(), leiras.end(), '_', ' ');
}
