#include "pch.h"
#include "Latnivalo.h"

Latnivalo::Latnivalo(const unsigned azon_, std::string_view nev_, const float hossz_, const float szel_, std::string_view katSzoveg_, const float atlIdo_, const std::string_view leiras_)
	: azon(azon_), nev(nev_), hossz(hossz_), katSzoveg(katSzoveg_), atlIdo(atlIdo_), leiras(leiras_)
{
	std::replace(nev.begin(), nev.end(), '_', ' ');
	std::replace(leiras.begin(), leiras.end(), '_', ' ');
	if(k)
}
