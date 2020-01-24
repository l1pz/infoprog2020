#pragma once

enum class Kategoria {
	muemlek, termKincs, muzeumInf, kilato
};

struct Latnivalo
{
	// Függvények
	Latnivalo(const unsigned azon, std::string_view nev, const float hossz, const float szel, std::string_view katSzoveg, const float atlIdo, const std::string_view leiras);
	Latnivalo() = default;
	inline bool operator< (const Latnivalo& masik) const{ return (this->azon < masik.azon); }

	// Változók
	unsigned azon;
	std::string nev; 
	float hossz; 
	float szel; 
	std::string katSzoveg;
	Kategoria kat; 
	float atlIdo; 
	std::string leiras;
};

