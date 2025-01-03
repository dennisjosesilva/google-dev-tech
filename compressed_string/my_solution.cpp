#include <iostream>

#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <stack>

class Printable
{
public:
  virtual void print() const = 0;
};

class PrintStr : public Printable
{
public:
  PrintStr(const std::string &in, int &idx);

  void print() const override;

private:
  std::string str_;
};

PrintStr::PrintStr(const std::string &in, int &idx)
{
  std::stringstream ss;
  do {
    ss << in[idx];
    idx++;
  } while (!isdigit(in[idx]) && in[idx] != ']');
  
  str_ = ss.str();
}

void PrintStr::print() const
{
  std::cout << str_;
}

class Repetition : public Printable
{
public:
  Repetition(const std::string &in, int &idx);
  ~Repetition();

  void print() const override;

private:
  Printable *parseRepetition(const std::string &in, int &idx) const;
  Printable *parsePrintString(const std::string &in, int &idx) const;

private:
  int nrep_;
  std::vector<Printable*> children_;
};

Repetition::Repetition(const std::string &in, int &idx)
{
  std::stringstream ss;
  // read nrep
  do {
    ss << in[idx];
    idx++;
  } while (isdigit(in[idx]));

  nrep_ = atoi(ss.str().c_str());
  idx++; // skip '[', we can have an assert in[idx] == '[' for well-formed expression

  // Parse children
  while (in[idx] != ']') {
    Printable *p;
    if (isdigit(in[idx])) 
      p = new Repetition(in, idx);
    else
      p = new PrintStr(in, idx);
    
    children_.push_back(p);    
  }
  idx++;
}

void Repetition::print() const
{
  for (int i = 0; i < nrep_; i++) {
    for (Printable *p : children_) {
      p->print();
    }
  }
}

Repetition::~Repetition()
{
  for (Printable *p : children_) {
    delete p;
    p = nullptr;
  }
}

std::unique_ptr<Printable> parseRepetition(const std::string &in, int &idx)
{
  return std::make_unique<Repetition>(in, idx);
}


int main(int argc, char *argv[])
{
  std::string instr{argv[1]};

  int sidx = 0;
  while (sidx < instr.size())
  {
    if (std::isdigit(instr[sidx])) {
      std::unique_ptr<Printable> p = parseRepetition(instr, sidx);
      p->print();
    } else {
      std::cout << instr[sidx];
      sidx++;
    }
  }
  
  std::cout << "\n";

  return 0;
}