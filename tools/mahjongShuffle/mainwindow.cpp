#include <iostream>
#include <gdkmm/pixbuf.h>
#include <algorithm>
 #include <fstream>

#include "handtree.h"
#include "handnode.h"
#include "mainwindow.h"
#include "possiblesets.h"

MainWindow::MainWindow()
: grid(), button("Get New Hand"){

  set_border_width(10);

  button.signal_clicked().connect(sigc::mem_fun(*this,
              &MainWindow::on_button_clicked));

  sortButton = Gtk::CheckButton("Sorted");
  sortButton.set_active();
  stopButton = Gtk::CheckButton("Stopped");
  stopButton.set_active(false);
  loopButton = Gtk::CheckButton("Loop");
  loopButton.set_active(false);

  stdformbutton = Gtk::CheckButton("Standard form");
  stdformbutton.set_active(false);

  

  path = "../../build/_deps/riichimahjongtiles-src/Regular";
  auto Hand = walls.TakeHand();
  Hand.push_back(walls.TakePiece());
  std::sort(Hand.begin(),Hand.end());
  isStdForm = Gtk::Label("Not Checked Yet");
  front = Gdk::Pixbuf::create_from_file("../../build/_deps/riichimahjongtiles-src/Regular/Front.svg");
  for(int i = 0; i < 14; i ++){
    int width = 100;
    auto top = Gdk::Pixbuf::create_from_file(getFilePath(Hand[i].toUint8_t()));
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
  grid.attach(button,5,1,2);
  grid.attach(sortButton,7,1);
  grid.attach(stdformbutton,10,1,2);
  grid.attach(isStdForm,2,1,3);
  grid.attach(stopButton,8,1,1);
  grid.attach(loopButton,9,1,1);
  grid.show_all();
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_button_clicked()
{
  if(stopButton.get_active()){
    return;
  }
  std::vector<Piece> Hand;
  do{
    if(stdformbutton.get_active()){
      Hand = GetPossibleStdFormHand();
    }else{
      walls = Walls();
      Hand = walls.TakeHand();
      Hand.push_back(walls.TakePiece());
    }
    if(sortButton.get_active()){
      std::sort(Hand.begin(),Hand.end());
    }
    std::string dotsStr = "";
    dots = (dots+1)%4;
    if(dots == 4){
      dots = 0;
    }
    if(dots == 1){
      dotsStr = ".";
    }
    if(dots == 2){
      dotsStr = "..";
    }
    if(dots == 3){
      dotsStr = "...";
    }
    Node* root = breakdownHand(Hand);
    if(root){
      std::ofstream os("hand.gv");
      root->DumpAsDot(os);
      os.close();
      
      if(root->leaves.size() > 0 && root->leaves[0]->type != Single && root->leaves[0]->type != Error){
        isStdForm.set_text("In Standard Form"+dotsStr);
      }else {
        isStdForm.set_text("Not Standard Form"+dotsStr);
      }
      delete root;
    }else{
      isStdForm.set_text("Error Case Found"+dotsStr);
      stopButton.set_active();
    }
  }while(loopButton.get_active() && !stopButton.get_active());
  for(int i = 0; i < 14; i ++){
    int width = 100;
    auto top = Gdk::Pixbuf::create_from_file(getFilePath(Hand[i].toUint8_t()));
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

const std::map<uint8_t,std::string> FILE_MAP = {
    {RED_DRAGON , "Chun.svg"},
    {WHITE_DRAGON , "Haku.svg"},
    {GREEN_DRAGON , "Hatsu.svg"},
    {ONE_CHARACTER , "Man1.svg"},
    {TWO_CHARACTER , "Man2.svg"},
    {THREE_CHARACTER , "Man3.svg"},
    {FOUR_CHARACTER , "Man4.svg"},
    {RED_FIVE_CHARACTER , "Man5-Dora.svg"},
    {FIVE_CHARACTER , "Man5.svg"},
    {SIX_CHARACTER , "Man6.svg"},
    {SEVEN_CHARACTER , "Man7.svg"},
    {EIGHT_CHARACTER , "Man8.svg"},
    {NINE_CHARACTER , "Man9.svg"},
    {SOUTH_WIND , "Nan.svg"},
    {NORTH_WIND , "Pei.svg"},
    {ONE_PIN , "Pin1.svg"},
    {TWO_PIN , "Pin2.svg"},
    {THREE_PIN , "Pin3.svg"},
    {FOUR_PIN , "Pin4.svg"},
    {RED_FIVE_PIN , "Pin5-Dora.svg"},
    {FIVE_PIN , "Pin5.svg"},
    {SIX_PIN , "Pin6.svg"},
    {SEVEN_PIN , "Pin7.svg"},
    {EIGHT_PIN , "Pin8.svg"},
    {NINE_PIN , "Pin9.svg"},
    {WEST_WIND , "Shaa.svg"},
    {ONE_BAMBOO , "Sou1.svg"},
    {TWO_BAMBOO , "Sou2.svg"},
    {THREE_BAMBOO , "Sou3.svg"},
    {FOUR_BAMBOO , "Sou4.svg"},
    {RED_FIVE_BAMBOO , "Sou5-Dora.svg"},
    {FIVE_BAMBOO , "Sou5.svg"},
    {SIX_BAMBOO , "Sou6.svg"},
    {SEVEN_BAMBOO , "Sou7.svg"},
    {EIGHT_BAMBOO , "Sou8.svg"},
    {NINE_BAMBOO , "Sou9.svg"},
    {EAST_WIND , "Ton.svg"},
};

auto MainWindow::getFilePath(Piece p) -> std::string{
 return path + "/" + (FILE_MAP.contains(p.raw_value()) ? FILE_MAP.at(p.raw_value()) : "Back.svg");
}
