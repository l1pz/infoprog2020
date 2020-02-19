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
  fm.div(R"~(vert <menuBar weight=28><sightList>)~");
  fm.caption("Látnivaló Kezelő");
  fm.events().unload([](const arg_unload &arg) {
    nana::msgbox mb(arg.window_handle, "Kilépés", msgbox::yes_no);
    mb.icon(mb.icon_question);
    mb << "Biztosan kilép?";
    arg.cancel = (mb.show() != msgbox::pick_yes);
  });
  menubar mb{fm};
  auto &filemenu = mb.push_back("Fájl");
  filemenu.append("Mentés", [fm](menu::item_proxy &ip) {
    SaveBinary(fs::current_path() / "data/latnivalok.bin");
  });
  filemenu.append_splitter();
  filemenu.append("Importálás CSV-ből", [fm](menu::item_proxy &ip) {
    filebox picker{fm, true};
    picker.allow_multi_select(false);
    auto paths = picker.show();
    if (!paths.empty()) {
      std::cout << paths.front() << std::endl;
      LoadCSV(paths.front());
    }
  });
  filemenu.append("Exportálás CSV-be", [fm](menu::item_proxy &ip) {
    filebox picker{fm, false};
    picker.allow_multi_select(false);
    auto paths = picker.show();
    if (!paths.empty()) {
      std::cout << paths.front() << std::endl;
      SaveCSV(paths.front());
    }
  });
  filemenu.append_splitter();
  filemenu.append("Kilépés", [fm](menu::item_proxy &ip) {
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
  lsbox.append_header("Név");
  auto value_translator = [](const std::vector<nana::listbox::cell> &cells) {
    return std::shared_ptr<Sight>();
  };
  auto cell_translator = [](const std::shared_ptr<Sight> s) {
    std::vector<nana::listbox::cell> cells;
    cells.emplace_back(s->name);
    return cells;
  };
  lsbox.at(0).shared_model<std::recursive_mutex>(sights, value_translator,
                                                 cell_translator);

  fm["menuBar"] << mb;
  fm["sightList"] << lsbox;
  fm.collocate();

  fm.show();
  exec();
}