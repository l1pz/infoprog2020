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
  });

  button addButton(fm);
  addButton.caption("Új hozzáadása");

  button modifyButton(fm);
  modifyButton.caption("Kijelölt módosítása");
  modifyButton.events().click([&list, &fm](const arg_click &arg) {
    if (list.selected().empty())
      return;
    auto selected = getSelectedSight(list);
    form modFm(fm, {500, 400});
    modFm.caption("Módosítás");
    modFm.div(
        R"~(
        <vert margin = 20
          <vert weight=85%
            <vert weight=40% gap=[0,20,repeated] vertForm>
            <vert margin=[20]
              <horFormTitle weight=30 gap=10>
              <horFormInput weight=30 gap=10>
            >
            <vert arrange=[30, variable] descForm>
          >
          <button margin=[20, 40%, 0, 40%]>
        >
        )~");

    label name(modFm);
    name.format(true);
    name.caption("<size=14>Név</>");

    textbox nameTb(modFm);
    nameTb.append(selected->name, false);

    label cat(modFm);
    cat.format(true);
    cat.caption("<size=14>Kategória</>");

    combox catCb(modFm);
    for (auto &category : categories) {
      catCb.push_back(category);
    }
    catCb.caption(selected->category);

    button save(modFm, "Mentés");

    label lat(modFm);
    lat.format(true);
    lat.text_align(align::center);
    lat.caption("<size=14>Szélesség</>");

    label lon(modFm);
    lon.format(true);
    lon.text_align(align::center);
    lon.caption("<size=14>Hosszúság</>");

    label time(modFm);
    time.format(true);
    time.text_align(align::center);
    time.caption("<size=14>Idő</>");

    textbox latTb(modFm);
    latTb.append(std::to_string(selected->latitude), false);

    textbox lonTb(modFm);
    lonTb.append(std::to_string(selected->longtitude), false);

    textbox timeTb(modFm);
    timeTb.append(std::to_string(selected->avgTime), false);

    label desc(modFm);
    desc.format(true);
    desc.caption("<size=14>Leírás</>");

    textbox descTb(modFm);
    descTb.multi_lines(true);
    descTb.line_wrapped(true);
    descTb.append(selected->description, false);

    modFm["vertForm"] << name << nameTb << cat << catCb;
    modFm["horFormTitle"] << lat << lon << time;
    modFm["horFormInput"] << latTb << lonTb << timeTb;
    modFm["descForm"] << desc << descTb;
    modFm["button"] << save;
    modFm.collocate();
    modFm.modality();
    modFm.show();
  });

  button showDescription(fm);
  showDescription.caption("Kijelölt leírása");
  showDescription.events().click([&list, &fm](const arg_click &arg) {
    if (list.selected().empty())
      return;
    auto selected = getSelectedSight(list);
    form descFm(fm, {600, 300});
    descFm.div("vert margin=20 <weight=30 header><body>");
    descFm.caption("Leírás");
    label header(descFm);
    textbox body(descFm);
    header.text_align(align::center);
    header.format(true);
    header.caption("<bold size=14>" + selected->name + "</>");
    body.editable(false);
    body.append(selected->description, false);
    body.line_wrapped(true);
    body.multi_lines(true);
    descFm["header"] << header;
    descFm["body"] << body;
    descFm.collocate();
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