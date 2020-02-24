#include "pch.h"

#include "RouteManager.hpp"
#include "SightsManager.hpp"

using namespace SightsManager;
using namespace RouteManager;
using namespace nana;

void handler(menu::item_proxy &ip) {
  msgbox menuBar("Menu Clicked ");
  menuBar << ip.text();
  menuBar.show();
};

std::shared_ptr<Sight> getSelectedSight(nana::listbox &list) {
  return list.at(0)
      .at(list.selected().front().item)
      .value<std::shared_ptr<Sight>>();
}

int main() {
  LoadBinary(fs::current_path() / "data/latnivalok.bin");
  form fm{API::make_center(1110, 600)};
  listbox list(fm);
  fm.div(
      R"~(
      vert 
      <menuBar weight=28>|
      < 
        <weight=70% sightList>
        <margin=5%<vert arrange=40 gap=15 buttons>>
      >
      )~");
  fm.caption("Látnivaló Kezelő");
  fm.events().unload([](const arg_unload &arg) {
    nana::msgbox menuBar(arg.window_handle, "Kilépés", msgbox::yes_no);
    menuBar.icon(menuBar.icon_question);
    menuBar << "Biztosan kilép?";
    arg.cancel = (menuBar.show() != msgbox::pick_yes);
  });
  menubar menuBar{fm};
  auto &fileMenu = menuBar.push_back("Fájl");
  fileMenu.append("Mentés", [fm](menu::item_proxy &ip) {
    SaveBinary(fs::current_path() / "data/latnivalok.bin");
  });
  fileMenu.append_splitter();
  fileMenu.append("Importálás CSV-ből", [&fm, &list](menu::item_proxy &ip) {
    filebox picker{fm, true};
    picker.allow_multi_select(false);
    auto paths = picker.show();
    if (!paths.empty()) {
      std::cout << paths.front() << std::endl;
      LoadCSV(paths.front());
      list.erase(0);
      for (auto &sight : sights) {
        list.at(0).append(*sight).value(sight);
      }
      list.column_at(0).fit_content();
    }
  });
  fileMenu.append("Exportálás CSV-be", [&fm](menu::item_proxy &ip) {
    filebox picker{fm, false};
    picker.allow_multi_select(false);
    auto paths = picker.show();
    if (!paths.empty()) {
      std::cout << paths.front() << std::endl;
      SaveCSV(paths.front());
    }
  });
  fileMenu.append_splitter();
  fileMenu.append("Kilépés", [&fm](menu::item_proxy &ip) {
    nana::msgbox exitMsg(fm, "Kilépés", msgbox::yes_no);
    exitMsg.icon(exitMsg.icon_question);
    exitMsg << "Biztosan kilép?";
    if (exitMsg.show() == msgbox::pick_yes) {
      API::exit();
    }
  });
  auto &helpmenu = menuBar.push_back("Segítség");
  helpmenu.append("Rólunk", handler);

  list.enable_single(true, true);
  list.append_header("Név");
  list.append_header("Kategória");
  list.append_header("Szélesség");
  list.append_header("Hosszúság");
  list.append_header("Idő");
  for (auto &sight : sights) {
    list.at(0).append(*sight).value(sight);
  }

  for (int i = 0; i < list.column_size(); i++) {
    list.column_at(i).fit_content();
  }

  list.events().selected([](const arg_listbox &arg) {
    if (arg.item.selected()) {
      arg.item.fgcolor(colors::blue);
    } else {
      arg.item.fgcolor(colors::black);
    };
  });

  button deleteButton(fm, {40, 100});
  deleteButton.caption("Kijelölt törlése");
  deleteButton.events().click([&list](const arg_click &arg) {
    if (list.selected().empty())
      return;
    auto selected = getSelectedSight(list);
    Delete(*selected);
    list.erase(list.selected());
    std::cout << sights.size() << std::endl;
  });

  button addButton(fm);
  addButton.caption("Új hozzáadása");

  button modifyButton(fm);
  modifyButton.caption("Kijelölt módosítása");

  button showDescription(fm);
  showDescription.caption("Kijelölt leírása");
  showDescription.events().click([&list, &fm](const arg_click &arg) {
    if (list.selected().empty())
      return;
    auto selected = getSelectedSight(list);
    form descFm(fm, {600, 300});
    descFm.caption("Leírás");
    label header(descFm, rectangle{20, 20, descFm.size().width - 20, 40});
    textbox body(descFm, rectangle{20, 50, descFm.size().width - 40,
                                   descFm.size().height - 70});
    header.text_align(align::center);
    header.format(true);
    header.caption("<bold>" + selected->name + "</>");
    body.editable(false);
    body.append(selected->description, false);
    body.line_wrapped(true);
    body.multi_lines(true);
    descFm.modality();
    descFm.show();
  });

  fm["menuBar"] << menuBar;
  fm["sightList"] << list;
  fm["buttons"] << showDescription;
  fm["buttons"] << modifyButton;
  fm["buttons"] << deleteButton;
  fm["buttons"] << addButton;
  fm.collocate();

  fm.show();
  nana::exec();
}