#pragma once
#include "pch.h"
#include "Latnivalo.h"
#include "IO.hpp"

namespace LatnivaloKezelo
{
static std::vector<std::shared_ptr<Latnivalo>> latnivalok;

std::optional<std::shared_ptr<Latnivalo>> KeresAzonosito(const unsigned azon)
{
	auto talalat{std::find_if(latnivalok.begin(), latnivalok.end(), [azon](std::shared_ptr<Latnivalo> latnivalo) {
		return latnivalo->azon == azon;
	})};
	if (talalat == latnivalok.end())
		return std::nullopt;
	return *talalat;
}

std::optional<std::vector<std::shared_ptr<Latnivalo>>> KeresKategoria(std::string_view kat)
{
	std::vector<std::shared_ptr<Latnivalo>> talalatok;
	for (auto latnivalo : latnivalok)
	{
		if (latnivalo->kat == kat)
			talalatok.emplace_back(latnivalo);
	}
	if (talalatok.empty())
		return std::nullopt;
	return talalatok;
}

std::optional<std::vector<std::shared_ptr<Latnivalo>>> KeresSzoveg(std::string_view szoveg)
{
	std::vector<std::shared_ptr<Latnivalo>> talalatok;

	for (auto latnivalo : latnivalok)
	{
		auto talalat = std::search(
			latnivalo->nev.begin(), latnivalo->nev.end(),
			szoveg.begin(), szoveg.end(),
			[](char a, char b) { return std::tolower(a) == std::tolower(b); });
		if (talalat != latnivalo->nev.end())
			talalatok.emplace_back(latnivalo);
	}

	if (talalatok.empty())
		return std::nullopt;

	return talalatok;
}

void Torol(const std::shared_ptr<Latnivalo> latnivalo)
{
	auto it = std::find(latnivalok.begin(), latnivalok.end(), latnivalo);
	latnivalok.erase(it);
	for (; it < latnivalok.end(); it++)
		(*it)->azon--;
}

void Hozzaad(const unsigned azon, std::string_view nev, const float hossz, const float szel, std::string_view katSzoveg, const float atlIdo, std::string_view leiras)
{
	std::shared_ptr<Latnivalo> latnivalo{std::make_shared<Latnivalo>(azon, nev, hossz, szel, katSzoveg, atlIdo, leiras)};
	latnivalok.emplace_back(latnivalo);
}

void Betolt(const std::string &fajlNev)
{
	std::ifstream fajlBe(fajlNev);
	if (fajlBe)
	{
		bool fejlecSor{true};
		std::string sor;
		while (std::getline(fajlBe, sor))
		{
			if (fejlecSor)
			{
				fejlecSor = false;
				continue;
			}
			if (sor.empty())
				continue;
			std::replace(sor.begin(), sor.end(), ' ', '_');
			std::replace(sor.begin(), sor.end(), ';', ' ');
			std::stringstream sorStream{sor};
			int azon;
			std::string nev;
			float hossz;
			float szel;
			std::string katSzoveg;
			float atlIdo;
			std::string leiras;
			sorStream >> azon >> nev >> hossz >> szel >> katSzoveg >> atlIdo >> leiras;
			Hozzaad(azon, nev, hossz, szel, katSzoveg, atlIdo, leiras);
		}
		fajlBe.close();
	}
	else {
		std::cout << "A latnivalok.csv hiányzik!" << std::endl;
		exit(-1);
	}
}
void Mentes(const std::string &fajlNev)
{
	std::ofstream fajlKi(fajlNev);
	fajlKi << "azonosito;nev;hosszusag;szelesseg;kategoria;atlagos_ido;leiras" << std::endl;
	std::cout.precision(6);
	for (auto latnivalo : latnivalok)
	{
		fajlKi << latnivalo->azon << ';' << latnivalo->nev << ';' << std::fixed << latnivalo->hossz << ';' << std::fixed << latnivalo->szel << ';' << latnivalo->kat << ';' << std::fixed << latnivalo->atlIdo << ';' << latnivalo->leiras << std::endl;
	}
	fajlKi.close();
}
void KepHozzaad(const std::shared_ptr<Latnivalo> latnivalo, const std::string_view fajlNev)
{
	latnivalo->kepek.emplace_back(fajlNev);
}

} // namespace LatnivaloKezelo
