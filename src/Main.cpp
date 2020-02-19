#include "pch.h"

#include "RouteManager.hpp"
#include "SightsManager.hpp"

using namespace SightsManager;
using namespace RouteManager;
using namespace nana;

void handler(menu::item_proxy &ip) {
  msgbox mb("Menu Clicked ");
  mb << ip.text();
  mb.show();
};

int main() {
  LoadBinary(fs::current_path() / "data/latnivalok.bin");
  form fm;
  fm.div(R"~(vert <menuBar weight=28><<weight=60% sightList><controlPanel>>)~");
  fm.caption("Látnivaló Kezelő");
  fm.events().unload([](const arg_unload &arg) {
    nana::msgbox mb(arg.window_handle, "Kilépés", msgbox::yes_no);
    mb.icon(mb.icon_question);
    mb << "Biztosan kilép?";
    arg.cancel = (mb.show() != msgbox::pick_yes);
  });
  menubar mb{fm};
  auto &fileMenu = mb.push_back("Fájl");
  fileMenu.append("Mentés", [fm](menu::item_proxy &ip) {
    SaveBinary(fs::current_path() / "data/latnivalok.bin");
  });
  fileMenu.append_splitter();
  fileMenu.append("Importálás CSV-ből", [fm](menu::item_proxy &ip) {
    filebox picker{fm, true};
    picker.allow_multi_select(false);
    auto paths = picker.show();
    if (!paths.empty()) {
      std::cout << paths.front() << std::endl;
      LoadCSV(paths.front());
    }
  });
  fileMenu.append("Exportálás CSV-be", [fm](menu::item_proxy &ip) {
    filebox picker{fm, false};
    picker.allow_multi_select(false);
    auto paths = picker.show();
    if (!paths.empty()) {
      std::cout << paths.front() << std::endl;
      SaveCSV(paths.front());
    }
  });
  fileMenu.enabled(3, false);
  fileMenu.append_splitter();
  fileMenu.append("Kilépés", [fm](menu::item_proxy &ip) {
    nana::msgbox mb(fm, "Kilépés", msgbox::yes_no);
    mb.icon(mb.icon_question);
    mb << "Biztosan kilép?";
    if (mb.show() == msgbox::pick_yes) {
      API::exit();
    }
  });
  auto &helpmenu = mb.push_back("Segítség");
  helpmenu.append("Rólunk", handler);

  listbox lsbox(fm);
  lsbox.enable_single(true, true);
  lsbox.append_header("Látványosság");
  for (auto &sight : sights) {
    lsbox.at(0).append(*sight).value(sight);
  }
  lsbox.column_at(0).fit_content();

  lsbox.events().selected([](const arg_listbox &arg) {
    if (arg.item.selected()) {
      arg.item.fgcolor(colors::blue);
    } else {
      arg.item.fgcolor(colors::black);
    };
  });

  button db(fm);
  db.caption("Törlés");
  db.events().click([&lsbox](const arg_click &arg) {
    if (lsbox.selected().empty())
      return;
    auto &selected = lsbox.at(0)
                         .at(lsbox.selected().front().item)
                         .value<std::shared_ptr<Sight>>();
    Delete(*selected);
    lsbox.erase(lsbox.selected());
    std::cout << sights.size() << std::endl;
  });

  fm["menuBar"] << mb;
  fm["sightList"] << lsbox;
  fm["controlPanel"] << db;
  fm.collocate();

  fm.show();
  nana::exec();
}