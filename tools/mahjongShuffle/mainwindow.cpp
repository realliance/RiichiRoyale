#include <iostream>
#include <gdkmm/pixbuf.h>
#include "mainwindow.h"

MainWindow::MainWindow()
: grid(), button("Get New Hand"){

  set_border_width(10);

  button.signal_clicked().connect(sigc::mem_fun(*this,
              &MainWindow::on_button_clicked));

  sortButton = Gtk::CheckButton("Sorted");
  sortButton.set_active();

  Walls::SetPath("../../riichi-mahjong-tiles/Regular");
  auto Hand = walls.TakeHand();
  Walls::Sort(Hand);
  front = Gdk::Pixbuf::create_from_file("../../riichi-mahjong-tiles/Regular/Front.svg");
  for(int i = 0; i < 14; i ++){
    int width = 100;
    auto top = Gdk::Pixbuf::create_from_file(Walls::GetPath(Hand[i]));
    auto piece = front->copy();
    top->composite(
      piece,0,0,top->get_width(),top->get_height(),
      150*0.1,
      width*((float)piece->get_width()/piece->get_height())*0.2,
      0.9,0.9,Gdk::INTERP_BILINEAR,255);
    imageArray.emplace_back(Gtk::Image(piece->scale_simple(
      width*((float)piece->get_width()/piece->get_height()),
      width,Gdk::INTERP_BILINEAR )));
    grid.attach(imageArray[i],i,0);
  }

  add(grid);
  grid.set_column_spacing(10);
  grid.set_row_spacing(10);
  grid.attach(button,6,1,2);
  grid.attach(sortButton,8,1);

  grid.show_all();
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_button_clicked()
{
  walls = Walls();
  auto Hand = walls.TakeHand();
  if(sortButton.get_active()){
    Walls::Sort(Hand);
  }
  for(int i = 0; i < 14; i ++){
    int width = 100;
    auto top = Gdk::Pixbuf::create_from_file(Walls::GetPath(Hand[i]));
    auto piece = front->copy();
    top->composite(
      piece,0,0,top->get_width(),top->get_height(),
      0,0,1,1,Gdk::INTERP_BILINEAR,255);
    imageArray[i].set(
      piece->scale_simple(
        width*((float)piece->get_width()/piece->get_height()),
        width,Gdk::INTERP_BILINEAR
      )
    );
  }
}
