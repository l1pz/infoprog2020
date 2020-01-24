#pragma once
#include "pch.h"
#include "Latnivalo.h"
#include "IO.hpp"

namespace LatnivaloKezelo {
	static std::vector<std::shared_ptr<Latnivalo>> latnivalok;

	std::optional<std::shared_ptr<Latnivalo>> KeresAzonosito(const unsigned azon) {
		auto talalat{ std::find_if(latnivalok.begin(), latnivalok.end(), [azon](std::shared_ptr<Latnivalo> latnivalo) {
			return latnivalo->azon == azon;
		}) };
		if (talalat == latnivalok.end()) return std::nullopt;
		return *talalat;
	}
	std::optional<std::vector<std::shared_ptr<Latnivalo>>> KeresKategoria(const Kategoria kat) {
		std::vector<std::shared_ptr<Latnivalo>> talalatok;
		for (auto latnivalo : latnivalok) {
			if (latnivalo->kat == kat) talalatok.emplace_back(latnivalo);
		}
		if (talalatok.empty()) return std::nullopt;
		return talalatok;
	}
	std::optional<std::vector<std::shared_ptr<Latnivalo>>> KeresSzoveg(std::string_view szoveg) {
		std::vector<std::shared_ptr<Latnivalo>> talalatok;
		for (auto latnivalo : latnivalok) {
			auto talalat = std::search(
				latnivalo->nev.begin(), latnivalo->nev.end(),
				szoveg.begin(), szoveg.end(),
				[](char a, char b) { return std::tolower(a) == std::tolower(b); }
			);
			if (talalat != latnivalo->nev.end()) talalatok.emplace_back(latnivalo);
		}
		if (talalatok.empty()) return std::nullopt;
		return talalatok;
	}
	void Hozzaad(const unsigned azon,const std::string_view nev, const float hossz, const float szel, std::string_view katSzoveg, const float atlIdo, std::string_view leiras) {
		std::shared_ptr<Latnivalo> latnivalo{ std::make_shared<Latnivalo>(azon, nev, hossz, szel, katSzoveg, atlIdo, leiras) };
		latnivalok.emplace_back(latnivalo);
	}
	void Beolvas(const std::string& fajlNev)
	{
		std::ifstream fajlBe(fajlNev);
		if (fajlBe) {
			bool fejlecSor{ true };
			std::string sor;
			while (std::getline(fajlBe, sor)) {
				if (fejlecSor) { fejlecSor = false; continue; }
				if (sor.empty()) continue;
				std::replace(sor.begin(), sor.end(), ' ', '_');
				std::replace(sor.begin(), sor.end(), ';', ' ');
				std::stringstream sorStream{ sor };
				int azon; std::string nev; float hossz; float szel; std::string katSzoveg; float atlIdo; std::string leiras;
				sorStream >> azon >> nev >> hossz >> szel >> katSzoveg >> atlIdo >> leiras;
				Hozzaad(azon, nev, hossz, szel, katSzoveg, atlIdo, leiras);
			}
		}
		else {
			std::cout << "A latnivalok.csv hiányzik!";
			exit(-1);
		}
	}
	

}

