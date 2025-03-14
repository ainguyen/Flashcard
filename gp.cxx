// generated by Fast Light User Interface Designer (fluid) version 1.0303

#include "gp.h"
#include <FL/Fl_PNG_Image.H>
#include <vector>
#include <iostream>
// {\tableofcontents\section{Specification}
// Our project is a software to help users learn vocabulary the same
// way like using flashcards.
// \\
// The interface of the software will be a window in which contains a 
// list of options to let users choose which option they want to use.
// \\
// The list of options may include :
// \begin{alltt}
//   - Choose how to show up the vocabulary in the list (consequently
//    or randomly) 
//   - How to show up the word needed to repeat ( by word or by time).
//     If there are some options to choose the number of words or the
//     amount of time.
//   - Users can edit the list of vocabulary right away by click on 
//     the pencil image
// \end{alltt}
// \\
//  }
// {\clearpage\section{Analysis}
// \begin{itemize}
// \item Input: the lists of word and their definition are loaded into 2 files:
//       word.txt and definition.txt. The word needs to be type in the form:
//       \\ 
//              01 word1
//       \\
//              02 word2
//       \\
//       and the same form for definition.
// \\             
// \item Output: The words and their definitions loaded into the label of two 
//       boxes in Panel window. Users can click Next if they already know the
//       word, or click repeat to show up the word after a specific amount of
//       time or after a specific number of word passed (users will choose this
//       at Setting window).
// \end{itemize}
// }
// {\clearpage\section{Design}
// \begin{alltt}
//  - Create 2 windows: Setting and Panel. Setting window is like the first
//    picture and Panel is like the other ones.
// \\
//  - Create radio buttons for the choices of "Show words" and 
//    how to "Repeat unknown words". Also use Fl_Choice to put in the numbers
//    of words and the amount of time that let users be able to choose.
// \\ 
//  - Create the button with a image (pencil) to let users be able to edit the
//    list of vocabulary when they click on it. Use function 
//    \[system("editor <filename>.txt &")\]
// \\
//  - Create 2 file notepad to store a list of vocabulary and a list of
//    definition in the same order ( word.txt and definition.txt)
// \\
//  - To load the list of vocabulary and list of definition, create a function
//    void load_file(string filename) using ifstream. Use while loop to read 
//    each line and store these vocabularies into wordvector and definitions 
//    into defvector by using <vectorname>.push_back() member function.
//    Also create load_num function (using ifstream,too) to return the number
//    of vocabulary ( it will be N).
// \\
//  - Create random(int a, int b) function with return the random value by 
//    using \[ rand() \% (b - a + 1) + a \] to show random order of words in
//    the list. So, when we call this function, it will be random(0,N).
// \\
//  - Create the struct {int wstep, int tstep, int mindex} to save the state 
//    of word needed to repeat when user click on repeat button.
// \\
//  - Create the function void show_repeat() make the words needed to repeat
//    show up after N_repeatword or after N_repeattime when users click next
//    button ( N_repeatword and N_repeattime depend on what users choose at
//    Setting window).
//    At first, wstep = N_repeatword or tstep = N_repeattime. By click the 
//    next button, wstep and tstep start to countdown to 0. Use if function
//    to check whether wstep or tstep == 0 to show up the words.
//    For the word countdown, just use some logic algorithm like decrement.
//    For the time countdown, create count_time(void*) and use 
//    \[Fl:add_timeout(1, count_time)\] and \[Fl::repeat_timeout(1,count_time)\]
//     to count the time every second.
// \\
//  - Create the function next_word(int i) to show how to show up the next word
// \end{alltt}
// }
// {\clearpage\section{Implementation}}
static int N; //the number of vocabularies in the list
static int N_repeatword; // number of words needed to be pass to show up the repeat word
static int N_repeattime; // amount of time needed to be pass to show up the repeat word
static int countWord = 0; 
static int cur_word = 0; 
static int repeat_order = 0; 
static vector<string> wordvector; 
static vector<string> defvector; 
struct repeatword {int wstep; int tstep; int mindex;}; 
static vector<repeatword> repeat_word; 

void Draw::draw() {
  Fl_Double_Window::draw();
  //draw two lines in Panel Window to seperate parts
  fl_color(FL_WHITE);
  int x1;
  int x2;
  int y1;
  int y2;
  // line 1 between word and definition
  x1 = Panel->w()/5; x2 = 4*Panel->w()/5;
  y1 = vocab->y() + vocab->h(); y2=y1;	
  fl_line(x1,y1,x2,y2);
  // line 2 between definition and buttons
  y1 = def->y() + def->h();  y2 = y1;
  fl_line(x1,y1,x2,y2);
}

void HelpDraw::draw() {
  Fl_Double_Window::draw();
  fl_color(FL_BLACK);
  fl_font(FL_HELVETICA,14);
  ifstream ifs("help.txt");
  string line;
  int scale = 0;
  while(getline(ifs, line))
  {
    scale += 20;
    fl_draw(line.c_str(), 20, scale);
  }
  ifs.close();
}

int load_num(string filename) {
  // get the total number of vocabularies that are in "word.txt"
  ifstream ifs(filename.c_str());
  string line;
  int num;
  while(getline(ifs, line))
  {
    string order = line.substr(0,2);
    num = atoi(order.c_str());
  }
  ifs.close();
  return num;
}

void load_file(string filename) {
  // load vocabularies and their definitions into wordvector and defvector
  ifstream ifs(filename.c_str());
  string line;
  while(getline(ifs, line))
  {
    string str = line.substr(3);
    if( filename == "word.txt" )
     wordvector.push_back(str);
    else
     defvector.push_back(str); 
  }
  ifs.close();
}

void load_repeat_value() {
  if(w->value(1)) {N_repeatword = 3;}
  else if(w->value(0)) {N_repeatword = 5;}
  if(t->value(1)) {N_repeattime = 10;}
  else if(t->value(0)) {N_repeattime = 30;}
}

int random(int a,int b) {
  // generate random number
  srand((unsigned int)time(0));
  return rand() % (b - a + 1) + a;
}

void show_repeat() {
  if(repeat_order > 0)
  {
    if (wchoice->value())
    {
      for(unsigned int i=0; i < repeat_word.size(); i++) 
      {repeat_word[i].wstep -= 1;
      
      cout << "Index " << i << " - Word " << 
               wordvector[repeat_word[i].mindex] <<
                " - Step left" << repeat_word[i].wstep << endl;}
      
      if(repeat_word[0].wstep == 0)
      {
        cur_word = repeat_word[0].mindex;
        
        cout << "--- Now repeat word = " << 
                wordvector[repeat_word[0].mindex] << endl;
        
        vocab->label(wordvector[repeat_word[0].mindex].c_str());
        def->label(defvector[repeat_word[0].mindex].c_str());
        
        repeat_word.erase(repeat_word.begin());
        repeat_order--;
        
        if(c->value()) {countWord--;}
      }
    }
    else if (tchoice->value())
    {
      if(repeat_word[0].tstep == 0)
      { 
        cur_word = repeat_word[0].mindex;
        
        cout << "--- Now repeat word = " << 
                wordvector[repeat_word[0].mindex] << endl;
        
        vocab->label(wordvector[repeat_word[0].mindex].c_str());
        def->label(defvector[repeat_word[0].mindex].c_str());
        
        repeat_word.erase(repeat_word.begin());
        
        for(unsigned int i=0; i < repeat_word.size(); i++) 
          {repeat_word[i].tstep = N_repeattime;}
        
        repeat_order--;
        
        if(c->value()) {countWord--;}
        // start to count time again when the first element in the repeat_vector removed
        Fl::add_timeout(1.0, count_time);
      }
    }
  }
}

void next_word(int i) {
  // how to show up the next vocabulary
  if(c->value())
  {
    vocab->label(wordvector[i].c_str());
    def->label(defvector[i].c_str());
  }
  else if(r->value())
  {
    i = random(0,N-1);
    vocab->label(wordvector[i].c_str());
    def->label(defvector[i].c_str());
  }
  cur_word = i;
  show->show();
  def->hide();
  nextWord->hide();
  repeat->hide();
  back->hide();
  // Call the show_repeat() function to check if there is any words needed to repeat
  show_repeat();
}

void choose_something() {
  // show errors message when users forget to choose any options
  if(!c->value() && !r->value() && !tchoice->value() && !wchoice->value())
  {
  error1->show();
  }
  if(!c->value() && !r->value() && (tchoice->value() || wchoice->value()))
  {
  error2->show();
  }
  if((c->value() || r->value()) && !tchoice->value() && !wchoice->value())
  {
  error3->show();
  }
  if((c->value() || r->value()) && (tchoice->value() || wchoice->value()))
  {
  Panel->show();
  Setting->hide();
  }
}

void count_time(void*) {
  if( repeat_word[0].tstep <= 0 ) // After N_repeattime seconds, turn timer off
  {Fl::remove_timeout(count_time);}
  else
  {
    repeat_word[0].tstep -= 1;
    cout << repeat_word[0].tstep << endl;
    Fl::repeat_timeout(1.0, count_time);
  }
}

Fl_Double_Window *error1=(Fl_Double_Window *)0;

static void cb_OK(Fl_Return_Button*, void*) {
  error1->hide();
}

Fl_Double_Window *error2=(Fl_Double_Window *)0;

static void cb_OK1(Fl_Return_Button*, void*) {
  error2->hide();
}

Fl_Double_Window *error3=(Fl_Double_Window *)0;

static void cb_OK2(Fl_Return_Button*, void*) {
  error3->hide();
}

HelpDraw *info=(HelpDraw *)0;

static void cb_Back(Fl_Return_Button*, void*) {
  info->hide();
}

Draw *Panel=(Draw *)0;

Fl_Box *background=(Fl_Box *)0;

Fl_Box *vocab=(Fl_Box *)0;

Fl_Button *show=(Fl_Button *)0;

static void cb_show(Fl_Button*, void*) {
  def->show();
nextWord->show();
repeat->show();
back->show();
show->hide();
show->clear_visible_focus();
}

Fl_Box *def=(Fl_Box *)0;

Fl_Button *nextWord=(Fl_Button *)0;

static void cb_nextWord(Fl_Button*, void*) {
  repeat->activate();
if(c->value()) 
{ 
  countWord++;
  if(countWord >= N) 
  { cout << "Run out of words. Start the words over automatically !" << endl;
  countWord = 0;}
}
next_word(countWord);
}

Fl_Button *repeat=(Fl_Button *)0;

static void cb_repeat(Fl_Button*, void*) {
  repeat->deactivate(); // avoid clicking repeat a word two times
repeat_order++;

repeatword abc;
abc.wstep = N_repeatword;
abc.tstep = N_repeattime;
abc.mindex = cur_word;
repeat_word.push_back(abc);
cout << "number of words to repeat: " << repeat_order << endl;
// start counting the time when users choose repeat 
//  after a specific amount of time
if(tchoice->value() && 
   repeat_word[0].tstep == N_repeattime)
{Fl::add_timeout(1.0, count_time);}
for(unsigned int i=0; i < repeat_word.size(); i++);
}

Fl_Button *back=(Fl_Button *)0;

static void cb_back(Fl_Button*, void*) {
  Setting->show();
Panel->hide();
start->label("Resume");
if(tchoice->value())
{Fl::remove_timeout(count_time);};
}

Fl_Double_Window *Setting=(Fl_Double_Window *)0;

Fl_Group *option1=(Fl_Group *)0;

Fl_Round_Button *c=(Fl_Round_Button *)0;

Fl_Round_Button *r=(Fl_Round_Button *)0;

Fl_Group *button=(Fl_Group *)0;

Fl_Button *start=(Fl_Button *)0;

static void cb_start(Fl_Button*, void*) {
  choose_something();
if(c->value())
{
  vocab->label(wordvector[countWord].c_str());
  def->label(defvector[countWord].c_str());
}
else if(r->value()) {next_word(countWord);}
repeat_word.clear();
repeat_order = 0;
cur_word = 0;
load_repeat_value();
}

Fl_Button *Refresh=(Fl_Button *)0;

static void cb_Refresh(Fl_Button*, void*) {
  wordvector.clear();
defvector.clear();
load_file("word.txt");
load_file("definition.txt");
N = load_num("word.txt");
cout << N << endl;
for(int i=0;i<N;i++){cout << wordvector[i] << ": " << defvector[i] << endl;};
}

Fl_Button *help=(Fl_Button *)0;

static void cb_help(Fl_Button*, void*) {
  info->show();
}

Fl_Button *Pencil=(Fl_Button *)0;

static void cb_Pencil(Fl_Button*, void*) {
  system("start word.txt");
system("start definition.txt");
}

Fl_Choice *t=(Fl_Choice *)0;

Fl_Choice *w=(Fl_Choice *)0;

Fl_Round_Button *tchoice=(Fl_Round_Button *)0;

Fl_Round_Button *wchoice=(Fl_Round_Button *)0;

int main(int argc, char **argv) {
  // load vocabularies, definition and the number of vocabularies
  load_file("word.txt");
  load_file("definition.txt");
  N = load_num("word.txt");
  cout << N << endl;
  for(int i=0;i<N;i++){cout << wordvector[i] << ": " << defvector[i] << endl;}
  { error1 = new Fl_Double_Window(310, 90, "Error");
    { new Fl_Box(65, 24, 185, 11, "Choose something to make program work !");
    } // Fl_Box* o
    { Fl_Return_Button* o = new Fl_Return_Button(120, 50, 70, 25, "OK");
      o->callback((Fl_Callback*)cb_OK);
    } // Fl_Return_Button* o
    error1->end();
  } // Fl_Double_Window* error1
  { error2 = new Fl_Double_Window(310, 90, "Error");
    { new Fl_Box(65, 19, 185, 11, "Choose how to show up vocabulary !");
    } // Fl_Box* o
    { Fl_Return_Button* o = new Fl_Return_Button(120, 50, 70, 25, "OK");
      o->callback((Fl_Callback*)cb_OK1);
    } // Fl_Return_Button* o
    error2->end();
  } // Fl_Double_Window* error2
  { error3 = new Fl_Double_Window(310, 90, "Error");
    { new Fl_Box(65, 24, 185, 11, "Choose how to repeat unknown words !");
    } // Fl_Box* o
    { Fl_Return_Button* o = new Fl_Return_Button(120, 50, 70, 25, "OK");
      o->callback((Fl_Callback*)cb_OK2);
    } // Fl_Return_Button* o
    error3->end();
  } // Fl_Double_Window* error3
  { info = new HelpDraw(0, 0, 465, 500);
    info->box(FL_FLAT_BOX);
    info->color(FL_BACKGROUND_COLOR);
    info->selection_color(FL_BACKGROUND_COLOR);
    info->labeltype(FL_NO_LABEL);
    info->labelfont(0);
    info->labelsize(14);
    info->labelcolor(FL_FOREGROUND_COLOR);
    info->align(Fl_Align(FL_ALIGN_TOP));
    info->when(FL_WHEN_RELEASE);
    { Fl_Return_Button* o = new Fl_Return_Button(375, 455, 67, 30, "Back");
      o->callback((Fl_Callback*)cb_Back);
    } // Fl_Return_Button* o
    info->end();
  } // HelpDraw* info
  { Panel = new Draw(0, 0, 344, 359);
    Panel->box(FL_FLAT_BOX);
    Panel->color(FL_BACKGROUND_COLOR);
    Panel->selection_color(FL_BACKGROUND_COLOR);
    Panel->labeltype(FL_NO_LABEL);
    Panel->labelfont(0);
    Panel->labelsize(14);
    Panel->labelcolor(FL_FOREGROUND_COLOR);
    Panel->align(Fl_Align(FL_ALIGN_TOP));
    Panel->when(FL_WHEN_RELEASE);
    { Fl_Box* o = background = new Fl_Box(0, 0, 344, 359);
      o->image (new Fl_PNG_Image("buttons/theme.png"));
    } // Fl_Box* background
    { vocab = new Fl_Box(70, 29, 220, 51);
      vocab->labelcolor(FL_BACKGROUND2_COLOR);
    } // Fl_Box* vocab
    { Fl_Button* o = show = new Fl_Button(120, 270, 100, 70, "Show definition");
      show->box(FL_NO_BOX);
      show->labelcolor((Fl_Color)48);
      show->callback((Fl_Callback*)cb_show);
      o->image (new Fl_PNG_Image("buttons/show.png"));
      o->shortcut(32); // shortcut : space bar
    } // Fl_Button* show
    { def = new Fl_Box(45, 102, 260, 133);
      def->labelcolor(FL_BACKGROUND2_COLOR);
      def->hide();
    } // Fl_Box* def
    { Fl_Button* o = nextWord = new Fl_Button(30, 270, 50, 60, "Next");
      nextWord->box(FL_NO_BOX);
      nextWord->labelcolor((Fl_Color)48);
      nextWord->callback((Fl_Callback*)cb_nextWord);
      nextWord->hide();
      o->image (new Fl_PNG_Image("buttons/next.png"));
      o->shortcut(49); // shortcut : press key "1"
    } // Fl_Button* nextWord
    { Fl_Button* o = repeat = new Fl_Button(140, 270, 50, 60, "Repeat");
      repeat->box(FL_NO_BOX);
      repeat->labelcolor((Fl_Color)48);
      repeat->callback((Fl_Callback*)cb_repeat);
      repeat->hide();
      o->image (new Fl_PNG_Image("buttons/repeat.png"));
      o->shortcut(50); // shortcut: press key "2"
    } // Fl_Button* repeat
    { Fl_Button* o = back = new Fl_Button(255, 270, 50, 60, "Setting");
      back->box(FL_NO_BOX);
      back->labelcolor((Fl_Color)48);
      back->callback((Fl_Callback*)cb_back);
      back->hide();
      o->image (new Fl_PNG_Image("buttons/gear.png"));
      o->shortcut(FL_BackSpace); // shortcut: press key "BackSpace"
    } // Fl_Button* back
    Panel->end();
  } // Draw* Panel
  { Setting = new Fl_Double_Window(405, 255, "Setting");
    { option1 = new Fl_Group(18, 21, 345, 25);
      { Fl_Round_Button* o = c = new Fl_Round_Button(143, 21, 115, 20, "consequently");
        c->type(102);
        c->down_box(FL_ROUND_DOWN_BOX);
        o->shortcut(99); // shortcut: key "c"
      } // Fl_Round_Button* c
      { r = new Fl_Round_Button(273, 21, 90, 25, "randomly");
        r->type(102);
        r->down_box(FL_ROUND_DOWN_BOX);
        r->shortcut(114); // shortcut: key "r"
      } // Fl_Round_Button* r
      { new Fl_Box(18, 21, 85, 21, "Show words");
      } // Fl_Box* o
      option1->end();
    } // Fl_Group* option1
    { button = new Fl_Group(35, 180, 265, 60);
      { Fl_Button* o = start = new Fl_Button(35, 180, 50, 60, "Start");
        start->box(FL_NO_BOX);
        start->callback((Fl_Callback*)cb_start);
        o->image (new Fl_PNG_Image("buttons/play.png"));
        o->shortcut(FL_Enter); // shortcut : press "Enter"
      } // Fl_Button* start
      { Fl_Button* o = Refresh = new Fl_Button(145, 180, 50, 60, "Refresh");
        Refresh->box(FL_NO_BOX);
        Refresh->callback((Fl_Callback*)cb_Refresh);
        o->image (new Fl_PNG_Image("buttons/refresh.png"));
        o->shortcut(FL_F+5); // shortcut : press "F5"
      } // Fl_Button* Refresh
      { Fl_Button* o = help = new Fl_Button(250, 180, 50, 60, "Help");
        help->box(FL_NO_BOX);
        help->callback((Fl_Callback*)cb_help);
        o->image (new Fl_PNG_Image("buttons/help.png"));
        o->shortcut(FL_F+1); // shortcut: press "F1"
      } // Fl_Button* help
      button->end();
    } // Fl_Group* button
    { Fl_Button* o = Pencil = new Fl_Button(315, 50, 70, 190);
      Pencil->box(FL_NO_BOX);
      Pencil->callback((Fl_Callback*)cb_Pencil);
      o->image(new Fl_PNG_Image("buttons/pencil.png"));
    } // Fl_Button* Pencil
    { Fl_Group* o = new Fl_Group(20, 83, 235, 65, "Repeat unknown words");
      o->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      { t = new Fl_Choice(175, 88, 80, 25, " after (in second)");
        t->down_box(FL_BORDER_BOX);
        t->add("10s");
        t->add("30s");
        t->value(0);
      } // Fl_Choice* t
      { w = new Fl_Choice(175, 123, 80, 25, " or after (in words)   ");
        w->down_box(FL_BORDER_BOX);
        w->add("3 words");
        w->add("5 words");
        w->value(0);
      } // Fl_Choice* w
      { Fl_Round_Button* o = tchoice = new Fl_Round_Button(20, 90, 15, 20);
        tchoice->type(102);
        tchoice->down_box(FL_ROUND_DOWN_BOX);
        o->shortcut(116); // shortcut: key "t"
      } // Fl_Round_Button* tchoice
      { Fl_Round_Button* o = wchoice = new Fl_Round_Button(20, 123, 15, 20);
        wchoice->type(102);
        wchoice->down_box(FL_ROUND_DOWN_BOX);
        o->shortcut(119); // shortcut: key "w"
      } // Fl_Round_Button* wchoice
      o->end();
    } // Fl_Group* o
    Setting->end();
  } // Fl_Double_Window* Setting
  Setting->show(argc, argv);
  return Fl::run();
}
// {\clearpage\section{Test}
// This is how the program looks like:
// \includegraphics[scale=0.6]{setting.png}
// \\
// \includegraphics[scale=0.6]{panela.png}
// \\
// \includegraphics[scale=0.6]{panelb.png}
// }
