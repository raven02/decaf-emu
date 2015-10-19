#pragma once
#include <cstdint>
#include <vector>
#include "instruction.h"
#include "instructionid.h"

// TODO: Move
struct BitRange
{
   int start;
   int end;
};

#define FLD(x, ...) x,
#define FLAGFLD(x, ...) x,
#define OPFLD(x, ...) x,
#define OPMASK(...)

enum class Field : uint32_t
{
   Invalid,
   #include "instructionfields.inl"
   FieldCount,
};

#undef FLD
#undef FLAGFLD
#undef OPFLD
#undef OPMASK

struct InstructionOpcode
{
   InstructionOpcode()
   {
   }

   InstructionOpcode(Field field, uint32_t value)
      : field(field), value(value)
   {
   }

   Field field = Field::Invalid;
   uint32_t value = 0;
};

struct InstructionData
{
   InstructionID id;
   std::string name;
   std::string fullname;
   std::vector<InstructionOpcode> opcode;
   std::vector<Field> read;
   std::vector<Field> write;
   std::vector<Field> flags;
};

struct InstructionAlias
{
   std::string name;
   InstructionID id;
   std::vector<InstructionOpcode> opcode;
};

struct InstructionTable
{
   void initialise();
   InstructionData *find(InstructionID instrId);
   InstructionData *decode(Instruction instr);
   Instruction encode(InstructionID id);
   InstructionAlias *findAlias(InstructionData *data, Instruction instr);
   bool isA(InstructionID id, Instruction instr);
};

extern InstructionTable gInstructionTable;

bool
isFieldMarker(Field field);

const char *
getFieldName(Field field);

uint32_t
getFieldStart(Field field);

uint32_t
getFieldEnd(Field field);

uint32_t
getFieldWidth(Field field);

uint32_t
getFieldBitmask(Field field);

template<InstructionID id>
bool
isA(Instruction instr)
{
   return gInstructionTable.isA(id, instr);
}
