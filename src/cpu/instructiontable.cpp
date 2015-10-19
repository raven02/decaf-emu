#include <cassert>
#include "bitutils.h"
#include "instructiondata.h"

InstructionTable gInstructionTable;

struct TableEntry
{
   struct FieldMap
   {
      Field field;
      std::vector<TableEntry> children;
   };

   void addInstruction(Field field, uint32_t value, InstructionData *instr)
   {
      auto fieldMap = getFieldMap(field);
      assert(fieldMap);
      assert(value < fieldMap->children.size());
      fieldMap->children[value].instr = instr;
   }

   void addTable(Field field)
   {
      if (!getFieldMap(field)) {
         fieldMaps.push_back({});
         auto &fieldMap = fieldMaps.back();
         auto size = 1 << getFieldWidth(field);
         fieldMap.field = field;
         fieldMap.children.resize(size);
      }
   }

   TableEntry *getEntry(Field field, uint32_t value)
   {
      auto fieldMap = getFieldMap(field);
      assert(fieldMap);
      assert(value < fieldMap->children.size());
      return &fieldMap->children[value];
   }

   FieldMap *getFieldMap(Field field)
   {
      for (auto &fieldMap : fieldMaps) {
         if (fieldMap.field == field) {
            return &fieldMap;
         }
      }

      return nullptr;
   }

   InstructionData *instr = nullptr;
   std::vector<FieldMap> fieldMaps;
};

static std::vector<InstructionData> instructionData;
static std::vector<InstructionAlias> aliasData;
static TableEntry instructionTable;

static void
initData();

static void
initTable();

#define FLD(x, y, z, ...) {y, z, #x},
#define FLAGFLD FLD
#define OPFLD FLD
#define OPMASK(...)
static struct {
   int start;
   int end;
   const char *name;
} gFieldData[] = {
   { -1, -1, "Invalid" },
   #include "instructionfields.inl"
};
#undef FLD
#undef FLAGFLD
#undef OPFLD
#undef OPMASK

bool
isFieldMarker(Field field)
{
   return gFieldData[static_cast<int>(field)].start == -1;
}

const char * getFieldName(Field field) {
   return gFieldData[static_cast<int>(field)].name;
}

// First bit of instruction field
uint32_t
getFieldStart(Field field)
{
   return 31 - gFieldData[static_cast<int>(field)].end;
}

// Last bit of instruction field
uint32_t
getFieldEnd(Field field)
{
   return 31 - gFieldData[static_cast<int>(field)].start;
}

// Width of instruction field in bits
uint32_t
getFieldWidth(Field field)
{
   return getFieldEnd(field) - getFieldStart(field) + 1;
}

// Absolute bitmask of instruction field
uint32_t
getFieldBitmask(Field field)
{
   return make_bitmask(getFieldStart(field), getFieldEnd(field));
}

uint32_t getFieldValue(const Field& field, Instruction instr)
{
   auto mask = getFieldBitmask(field);
   auto start = getFieldStart(field);
   auto res = (instr & mask) >> start;
   if (field == Field::spr) {
      res = ((res << 5) & 0x3E0) | ((res >> 5) & 0x1F);
   }
   return res;
}

InstructionData *
InstructionTable::find(InstructionID instrId)
{
   return &instructionData[static_cast<size_t>(instrId)];
}

// Decode Instruction to InstructionData
InstructionData *
InstructionTable::decode(Instruction instr)
{
   TableEntry *table = &instructionTable;
   InstructionData *data = nullptr;

   while (table) {
      for (auto &fieldMap : table->fieldMaps) {
         auto value = getFieldValue(fieldMap.field, instr);
         table = &fieldMap.children[value];

         if (table->instr || table->fieldMaps.size()) {
            break;
         }
      }

      if (table->fieldMaps.size() == 0) {
         return table->instr;
      }
   }

   return nullptr;
}

InstructionAlias *
InstructionTable::findAlias(InstructionData *data, Instruction instr)
{
   return nullptr;
   /*
   for (auto &alias : aliasData) {
      if (alias.id != data->id) {
         // Not an alias for this field
         continue;
      }

      bool opMatch = true;
      for (auto &op : alias.opcode) {
         uint32_t x = getFieldValue(op.field, instr);
         uint32_t y = op.value;
         if (op.field2 != Field::Invalid) {
            y = getFieldValue(op.field2, instr);
         }

         if (x != y) {
            opMatch = false;
            break;
         }
      }

      if (opMatch) {
         return &alias;
      }
   }
   return nullptr;
   */
}

// Check if is a specific instruction
bool
InstructionTable::isA(InstructionID id, Instruction instr)
{
   auto &data = instructionData[static_cast<size_t>(id)];

   for (auto &op : data.opcode) {
      auto field = op.field;
      auto value = op.value;
      auto start = getFieldStart(field);
      auto mask = getFieldBitmask(field);

      if (((instr.value & mask) >> start) != value) {
         return false;
      }
   }

   return true;
}

Instruction
InstructionTable::encode(InstructionID id)
{
   uint32_t instr = 0;
   auto &data = instructionData[static_cast<size_t>(id)];

   for (auto &op : data.opcode) {
      auto field = op.field;
      auto value = op.value;
      auto start = getFieldStart(field);

      instr |= value << start;
   }

   return instr;
}

void
InstructionTable::initialise()
{
   initData();
   initTable();
}

// Initialise instructionTable
void
initTable()
{
   /*
   for (auto &instr : instructionData) {
      TableEntry *table = &instructionTable;

      // Resolve opcodes
      for (auto i = 0u; i < instr.opcode.size() - 1; ++i) {
         auto field = instr.opcode[i].field;
         auto value = instr.opcode[i].value;
         table->addTable(field);
         table = table->getEntry(field, value);
      }

      // Add the actual instruction entry
      auto field = instr.opcode.back().field;
      auto value = instr.opcode.back().value;
      table->addTable(instr.opcode.back().field);
      table->addInstruction(field, value, &instr);
   }
   */
}

std::string cleanInsName(const std::string& name)
{
   if (name[name.size() - 1] == '_') {
      return name.substr(0, name.size() - 1);
   }
   return name;
}

struct EffectList;

struct FieldIndex {
   FieldIndex(Field _id) : id(_id) { }
   operator Field() const { return id; }
   Field id;

   EffectList eq(uint32_t value, const EffectList& lhs) const;
};

struct OpFieldIndex {
   OpFieldIndex(Field _id) : id(_id) { }
   Field id;
};

struct FlagFieldIndex {
   FlagFieldIndex(Field _id) : id(_id) { }
   operator Field() const { return id; }
   Field id;

   EffectList on(const EffectList& lhs) const;
};

struct InstructionMaskBits
{
   InstructionMaskBits(uint32_t _start, uint32_t _end) {
      start = _start;
      end = _end;
   }

   uint32_t start;
   uint32_t end;

   operator InstructionOpcode() const
   {
      return InstructionOpcode(Field::OE, 0);
   }

   InstructionOpcode operator !() const
   {
      // TODO: Fix This!
      return InstructionOpcode(Field::OE, 0);
   }

};

enum StateFlagIdx {
   Invalid,
   CR_all,
   CR_field,
   CR_LT,
   CR_GT,
   CR_EQ,
   CR_SO,
   CR_UN,
   CR_FX,
   CR_FEX,
   CR_VX,
   CR_OX,
   XER_SO,
   XER_OV,
   XER_CA,
   XER_CRXR,
   FPSCR_all,
   FPSCR_field,
   FPSCR_FPCC,
   FPSCR_FX,
   FPSCR_VXSNAN,
   FPSCR_VXVC,
   FPSCR_FPRF,
   FPSCR_FR,
   FPSCR_FI,
   FPSCR_OX,
   FPSCR_UX,
   FPSCR_XX,
   FPSCR_VXIS,
   FPSCR_ZX,
   FPSCR_VXISI,
   FPSCR_VXIDI,
   FPSCR_VXZDZ,
   FPSCR_VXIMZ,
   FPSCR_VXSOFT,
   FPSCR_VXSQRT,
   FPSCR_VXCVI,
   FPSCR_VE,
};
typedef std::vector<StateFlagIdx> StateFlagIdxList;

enum GenericFlagIdx {
   //--CR
   LT,
   GT,
   EQ,
   UN,
   SO,
   FX,
   FEX,
   VX,
   OX,
   //--XER
   OV,
   //SO,
   CA,
   CRXR,
   //--FPSCR
   FPCC,
   //FX,
   //VX,
   VXSNAN,
   VXVC,
   FPRF,
   FR,
   FI,
   //FEX,
   //OX,
   UX,
   XX,
   VXIS,
   ZX,
   VXISI,
   VXIDI,
   VXZDZ,
   VXIMZ,
   VXSOFT,
   VXSQRT,
   VXCVI,
   VE,
};
typedef std::vector<GenericFlagIdx> GenericFlagIdxList;

static GenericFlagIdxList
operator,(const GenericFlagIdx& lhs, const GenericFlagIdx& rhs) {
   return { lhs, rhs };
}
static GenericFlagIdxList
operator,(const GenericFlagIdxList& lhs, const GenericFlagIdx& rhs) {
   GenericFlagIdxList list = lhs;
   list.push_back(rhs);
   return list;
}

static InstructionOpcode
operator==(const OpFieldIndex &lhs, const int &rhs)
{
   return InstructionOpcode(lhs.id, rhs);
}


enum class TargetType {
   StateFlag,
   MaskedStateField,
   GPR,
   FPR,
   GQR,
   PSR0,
   PSR1,
   MEM_GPR_PLUS_GPR,
   MEM_GPR_PLUS_SIMM,
   GQRMEM_GPR_PLUS_GPR,
   GQRMEM_GPR_PLUS_SIMM,
};

struct EffectList;

enum class EffectConditionType
{
   OpFlagSet,
   FieldMatch
};

struct EffectCondition
{
   EffectCondition(EffectConditionType _type)
      : type(_type) { }

   EffectConditionType type;
   union {
      struct { // OpFlagSet
         FlagFieldIndex flag;
      };
      struct {
         FieldIndex field;
         uint32_t value;
      };
   };
};

struct EffectInfo
{
   TargetType type;
   std::vector<EffectCondition> conditions;
   union {
      struct { // MEM
         Field field1;
         Field field2;
         uint32_t size;
      };
      struct { // GPR, FPR
         Field index;
      };
      struct { // StateFlag
         StateFlagIdx field;
         Field shiftField;
         uint32_t shiftMulti;
      };
      struct { // MaskedStateField
         StateFlagIdx maskTarget;
         FieldIndex maskField;
      };
   };

   EffectInfo() { }

};

struct EffectList
{
   EffectList() {
   }

   EffectList(std::initializer_list<EffectList> lists) {
      for (auto &list : lists) {
         for (auto &i : list.effects) {
            effects.push_back(i);
         }
      }
   }

   EffectList(std::initializer_list<EffectInfo> list) {
      for (auto &i : list) {
         effects.push_back(i);
      }
   }

   EffectList(const StateFlagIdxList& list) {
      for (auto i : list) {
         EffectInfo info;
         info.type = TargetType::StateFlag;
         info.field = i;
         effects.push_back(info);
      }
   }

   std::vector<EffectInfo> effects;
};

struct GPRRef {
   GPRRef() : field(Field::Invalid) { }
   GPRRef(FieldIndex _field) : field(_field) { }
   FieldIndex field;

   operator EffectList() {
      EffectInfo info;
      info.type = TargetType::GPR;
      info.index = field.id;
      return { info };
   }
};

struct GPRArithmetic {
   GPRArithmetic(const GPRRef& lhs, const FieldIndex& f)
      : left(lhs), rightField(f) { }

   GPRArithmetic(const GPRRef& lhs, const GPRRef& rhs)
      : left(lhs), rightField(Field::Invalid), right(rhs) { }

   GPRRef left;
   FieldIndex rightField;
   GPRRef right;

};

GPRArithmetic operator+(const GPRRef& lhs, const FieldIndex& f) {
   return GPRArithmetic(lhs, f);
}
GPRArithmetic operator+(const GPRRef& lhs, const GPRRef& rhs) {
   return GPRArithmetic(lhs, rhs);
}

EffectList MEM(const GPRArithmetic& target, uint32_t size) {
   EffectInfo info;
   if (target.rightField.id == Field::Invalid) {
      info.type = TargetType::MEM_GPR_PLUS_GPR;
      info.field1 = target.left.field;
      info.field2 = target.right.field;
      info.size = size;
   } else {
      info.type = TargetType::MEM_GPR_PLUS_SIMM;
      info.field1 = target.left.field;
      info.field2 = target.rightField;
      info.size = size;
   }
   return{ info };
}

FieldIndex EXTS(const FieldIndex& field) {
   // For now, all fields are assumed to be signed extended for this purpose
   return field;
}

struct {
   GPRRef operator[](const FieldIndex& lhs) {
      return GPRRef(lhs);
   }
} GPR;
struct {
   GPRRef operator[](const FieldIndex& lhs) {
      return GPRRef(lhs);
   }
} GPRZ;

struct FieldMask {
   FieldMask(const FieldIndex& _field) : field(_field) { }
   FieldIndex field;
};
FieldMask FIELDMASK(const FieldIndex& field) {
   return FieldMask(field);
}

struct FPRRef {
   FPRRef(FieldIndex _field) : field(_field) { }
   FieldIndex field;

   operator EffectList() {
      EffectInfo info;
      info.type = TargetType::FPR;
      info.index = field.id;
      return{ info };
   }
};
struct {
   FPRRef operator[](const FieldIndex& lhs) {
      return FPRRef(lhs);
   }
} FPR;

struct PSRRef {
   PSRRef(FieldIndex _field, uint32_t _which) 
      : field(_field), which(_which) { }
   FieldIndex field;
   uint32_t which;

   operator EffectList() {
      EffectList list;
      if (which & 1) {
         EffectInfo info;
         info.type = TargetType::PSR0;
         info.index = field.id;
         list.effects.push_back(info);
      }
      if (which & 2) {
         EffectInfo info;
         info.type = TargetType::PSR1;
         info.index = field.id;
         list.effects.push_back(info);
      }
      return list;
   }
};
struct {
   PSRRef operator[](const FieldIndex& lhs) {
      return PSRRef(lhs, 1);
   }
} PSR0;
struct {
   PSRRef operator[](const FieldIndex& lhs) {
      return PSRRef(lhs, 2);
   }
} PSR1;
struct {
   PSRRef operator[](const FieldIndex& lhs) {
      return PSRRef(lhs, 1|2);
   }
} PSR;

struct GQRRef {
   GQRRef(FieldIndex _field) : field(_field) { }
   FieldIndex field;

   EffectInfo effectInfo() const {
      EffectInfo info;
      info.type = TargetType::GQR;
      info.index = field.id;
      return info ;
   }

   operator EffectList() const {
      return { effectInfo() };
   }

   EffectList MEM(const GPRArithmetic& target, uint32_t size) const {
      EffectInfo info;
      if (target.rightField.id == Field::Invalid) {
         info.type = TargetType::GQRMEM_GPR_PLUS_GPR;
         info.field1 = target.left.field;
         info.field2 = target.right.field;
         info.size = size; // number of paired singles to load (1 or 2)
      } else {
         info.type = TargetType::GQRMEM_GPR_PLUS_SIMM;
         info.field1 = target.left.field;
         info.field2 = target.rightField;
         info.size = size; // number of paired singles to load (1 or 2)
      }
      return { effectInfo(), info };
   }

};
struct {
   GQRRef operator[](const FieldIndex& lhs) {
      return GQRRef(lhs);
   }
} GQR;

struct StateFlagBuilder {
   StateFlagBuilder()
      :shiftField(Field::Invalid), shiftMulti(0) { }
   StateFlagBuilder(uint32_t offset)
      : shiftField(Field::Invalid), shiftMulti(offset) { }
   StateFlagBuilder(Field _shiftField, uint32_t _shiftMulti)
      : shiftField(_shiftField), shiftMulti(_shiftMulti) { }

   Field shiftField;
   uint32_t shiftMulti;

   virtual StateFlagIdx flagIdx(const GenericFlagIdx &id) const = 0;

   EffectInfo makeInfo(const GenericFlagIdx &id) const {
      EffectInfo info;
      info.type = TargetType::StateFlag;
      info.field = flagIdx(id);
      info.shiftField = shiftField;
      info.shiftMulti = shiftMulti;
      return info;
   }

   EffectList operator[](const GenericFlagIdx &id) const {
      EffectList list;
      list.effects.push_back(makeInfo(id));
      return list;
   }

   EffectList operator[](const GenericFlagIdxList &idList) const {
      EffectList out;
      for (auto i : idList) {
         out.effects.push_back(makeInfo(i));
      }
      return out;
   }
};
struct : StateFlagBuilder {
   StateFlagIdx flagIdx(const GenericFlagIdx &id) const override {
      switch (id) {
      case GenericFlagIdx::SO:
         return StateFlagIdx::XER_SO;
      case GenericFlagIdx::OV:
         return StateFlagIdx::XER_OV;
      case GenericFlagIdx::CA:
         return StateFlagIdx::XER_CA;
      case GenericFlagIdx::CRXR:
         return StateFlagIdx::XER_CRXR;
      default:
         throw;
      }
   }
} XER;
struct CRBuilder : StateFlagBuilder {
   CRBuilder() : StateFlagBuilder() { }
   CRBuilder(uint32_t crfId) 
      : StateFlagBuilder(crfId * 4) { }
   CRBuilder(Field field) 
      : StateFlagBuilder(field, 4) { }

   StateFlagIdx flagIdx(const GenericFlagIdx &id) const override {
      switch (id) {
      case GenericFlagIdx::LT:
         return StateFlagIdx::CR_LT;
      case GenericFlagIdx::GT:
         return StateFlagIdx::CR_GT;
      case GenericFlagIdx::EQ:
         return StateFlagIdx::CR_EQ;
      case GenericFlagIdx::SO:
         return StateFlagIdx::CR_SO;
      case GenericFlagIdx::UN:
         return StateFlagIdx::CR_UN;
      case GenericFlagIdx::FX:
         return StateFlagIdx::CR_FX;
      case GenericFlagIdx::FEX:
         return StateFlagIdx::CR_FEX;
      case GenericFlagIdx::VX:
         return StateFlagIdx::CR_VX;
      case GenericFlagIdx::OX:
         return StateFlagIdx::CR_OX;
      default:
         throw;
      }
   }

   operator EffectList() const {
      EffectInfo info;
      info.type = TargetType::StateFlag;
      info.field = StateFlagIdx::CR_field;
      info.shiftField = shiftField;
      info.shiftMulti = 4;
      return{ info };
   }
};
struct {
   CRBuilder operator[](const uint32_t& crfId) {
      return CRBuilder(crfId);
   }
   CRBuilder operator[](const Field& field) {
      return CRBuilder(field);
   }

   EffectList operator[](const FieldMask& mask) const {
      EffectInfo info;
      info.type = TargetType::MaskedStateField;
      info.maskTarget = StateFlagIdx::CR_field;
      info.maskField = mask.field;
      return{ info };
   }

   operator EffectList() const {
      EffectInfo info;
      info.type = TargetType::StateFlag;
      info.field = StateFlagIdx::CR_all;
      info.shiftField = Field::Invalid;
      info.shiftMulti = 1;
      return{ info };
   }
} CR;
struct : public StateFlagBuilder {
   StateFlagIdx flagIdx(const GenericFlagIdx &id) const override {
      switch (id) {
      case GenericFlagIdx::FPRF:
         return StateFlagIdx::FPSCR_FPRF;
      case GenericFlagIdx::VXVC:
         return StateFlagIdx::FPSCR_VXVC;
      case GenericFlagIdx::FPCC:
         return StateFlagIdx::FPSCR_FPCC;
      case GenericFlagIdx::FR:
         return StateFlagIdx::FPSCR_FR;
      case GenericFlagIdx::FI:
         return StateFlagIdx::FPSCR_FI;
      case GenericFlagIdx::FX:
         return StateFlagIdx::FPSCR_FX;
      case GenericFlagIdx::OX:
         return StateFlagIdx::FPSCR_OX;
      case GenericFlagIdx::UX:
         return StateFlagIdx::FPSCR_UX;
      case GenericFlagIdx::XX:
         return StateFlagIdx::FPSCR_XX;
      case GenericFlagIdx::VXSNAN:
         return StateFlagIdx::FPSCR_VXSNAN;
      case GenericFlagIdx::VXIS:
         return StateFlagIdx::FPSCR_VXIS;
      case GenericFlagIdx::ZX:
         return StateFlagIdx::FPSCR_ZX;
      case GenericFlagIdx::VXISI:
         return StateFlagIdx::FPSCR_VXISI;
      case GenericFlagIdx::VXIDI:
         return StateFlagIdx::FPSCR_VXIDI;
      case GenericFlagIdx::VXZDZ:
         return StateFlagIdx::FPSCR_VXZDZ;
      case GenericFlagIdx::VXIMZ:
         return StateFlagIdx::FPSCR_VXIMZ;
      case GenericFlagIdx::VXSOFT:
         return StateFlagIdx::FPSCR_VXSOFT;
      case GenericFlagIdx::VXSQRT:
         return StateFlagIdx::FPSCR_VXSQRT;
      case GenericFlagIdx::VXCVI:
         return StateFlagIdx::FPSCR_VXCVI;
      case GenericFlagIdx::VE:
         return StateFlagIdx::FPSCR_VE;
      default:
         throw;
      }
   }

   EffectList operator[](const GenericFlagIdx &id) const {
      return StateFlagBuilder::operator[](id);
   }
   EffectList operator[](const GenericFlagIdxList &idList) const {
      return StateFlagBuilder::operator[](idList);
   }
   EffectList operator[](const FieldIndex& field) const {
      EffectInfo info;
      info.type = TargetType::StateFlag;
      info.field = StateFlagIdx::FPSCR_field;
      info.shiftField = field;
      info.shiftMulti = 4;
      return { info };
   }
   EffectList operator[](const FieldMask& mask) const {
      EffectInfo info;
      info.type = TargetType::MaskedStateField;
      info.maskTarget = StateFlagIdx::FPSCR_field;
      info.maskField = mask.field;
      return { info };
   }

   operator EffectList() const {
      EffectInfo info;
      info.type = TargetType::StateFlag;
      info.field = StateFlagIdx::FPSCR_all;
      info.shiftField = Field::Invalid;
      info.shiftMulti = 1;
      return{ info };
   }

} FPSCR;

EffectList FieldIndex::eq(uint32_t value, const EffectList& lhs) const
{
   EffectList list = lhs;
   for (auto &i : list.effects) {
      EffectCondition cond(EffectConditionType::FieldMatch);
      cond.field = *this;
      cond.value = value;
      i.conditions.push_back(cond);
   }
   return list;
}

EffectList FlagFieldIndex::on(const EffectList& lhs) const {
   EffectList list = lhs;
   for (auto &i : list.effects) {
      EffectCondition cond(EffectConditionType::OpFlagSet);
      cond.flag = *this;
      i.conditions.push_back(cond);
   }
   return list;
}

// Initialise instructionData
#define FLD(x, ...) \
   static const FieldIndex x(Field::x);
#define FLAGFLD(x, ...) \
   static const FlagFieldIndex x(Field::x);
#define OPFLD(x, ...) \
   static const OpFieldIndex x(Field::x);
#define OPMASK(x, s, e) \
   static const InstructionMaskBits x(s, e);

struct InstructionBadOpcode
{
   InstructionBadOpcode()
   {
   }

   InstructionBadOpcode(Field field, uint32_t value)
      : field1(field), field2(Field::Invalid), value(value)
   {
   }

   InstructionBadOpcode(Field _field1, Field _field2)
      : field1(_field1), field2(_field2)
   {
   }

   Field field1;
   Field field2;
   uint32_t value;
};

InstructionBadOpcode operator==(const FieldIndex& field, const int& value) {
   return InstructionBadOpcode(field, value);
}

InstructionBadOpcode operator==(const FieldIndex& field1, const FieldIndex& field2) {
   return InstructionBadOpcode(field1, field2);
}

enum class InstructionType {
   User,
   Branch,
   Special,
   Super,
};
const InstructionType USER = InstructionType::User;
const InstructionType BRANCH = InstructionType::Branch;
const InstructionType SPECIAL = InstructionType::Special;
const InstructionType SUPER = InstructionType::Super;

struct InstructionDataBuilder
{
   InstructionID id;
   InstructionType mode;
   std::string name;
   std::string fullname;
   std::vector<InstructionOpcode> opcode;
   std::vector<InstructionBadOpcode> badops;
   std::vector<Field> read;
   std::vector<Field> write;
   std::vector<Field> flags;
   EffectList reads;
   EffectList writes;

   operator InstructionData() const {
      return InstructionData{
        InstructionID::add, "", "", {}, {}, {}, {}
      };
   }
};

#include "instructionfields.inl"
#undef FLD
#undef MRKR

#define PRINTOPS(...) __VA_ARGS__
#define INS(name, write, read, flags, opcodes, badops, mode, fullname, writelist, readlist) \
   InstructionDataBuilder ins_##name = { \
      InstructionID::name, mode, cleanInsName(#name), fullname, \
      { PRINTOPS opcodes }, { PRINTOPS badops }, \
      { PRINTOPS read }, { PRINTOPS write }, { PRINTOPS flags }, \
      { PRINTOPS readlist }, { PRINTOPS writelist } }; \
      instructionData.emplace_back(ins_##name);

#define INSA(name, op, opcodes)

void
initData()
{
   #include "instructions.inl"
//#include "instructionaliases.inl"
   /*
   INS(add, (rD), (rA, rB), (OE, Rc), (opcd == 31, xo2 == 266), (), USER, "Add",
      (GPR[rD], Rc.on(CR[0][LT, GT, EQ, SO]), OE.on(XER[SO, OV])),
      (GPR[rA], GPR[rB]))

   INS(lbz, (rD), (rA, d), (), (opcd == 34), (), USER, "Load Byte and Zero",
      (GPR[rD]),
      (MEM(GPRZ[rA] + EXTS(d), 1)))

   INS(fcmpo, (crfD), (frA, frB), (), (opcd == 63, xo1 == 32, !_9_10, !_31), (), USER, "Floating Compare Ordered",
      (CR[crfD][LT, GT, EQ, UN], FPSCR[FPCC, FX, VXSNAN, VXVC]),
      (FPR[rA], FPR[rB]))

   INS(mcrfs, (crfD), (crfS), (), (opcd == 63, xo1 == 64, !_9_10, !_14_15, !_16_20, !_31), (), USER, "",
      (CR[crfD], 
         crfS.eq(0, FPSCR[FX,OX]), 
         crfS.eq(1, FPSCR[UX,ZX,XX,VXSNAN]),
         crfS.eq(2, FPSCR[VXISI,VXIDI,VXZDZ,VXIMZ]),
         crfS.eq(3, FPSCR[VXVC]),
         crfS.eq(5, FPSCR[VXSOFT,VXSQRT,VXCVI])),
      (FPSCR[crfS]))

   INS(mffs, (frD), (), (Rc), (opcd == 63, xo1 == 583, !_11_15, !_16_20), (), USER, "",
      (FPR[rD], Rc.on(CR[1][FX, FEX, VX, OX])),
      (FPSCR))

   INS(mtfsf, (), (fm, frB), (Rc), (opcd == 63, xo1 == 711, !_6, !_15), (), USER, "",
      (FPSCR[FIELDMASK(fm)], Rc.on(CR[1][FX,FEX,VX,OX])),
      (FPR[rB]))
   INS(mtfsfi, (crfD), (), (Rc, imm), (opcd == 63, xo1 == 134, !_9_10, !_11_15, !_20), (), USER, "",
      (FPSCR[crfD], Rc.on(CR[1][FX,FEX,VX,OX])),
      ())

   INS(ps_add, (frD), (frA, frB), (Rc), (opcd == 4, xo4 == 21, !_21_25), (), USER, "Paired Single Add",
      (PSR[rD], FPSCR[FPRF,FR,FI,FX,OX,UX,XX,VXSNAN,VXISI], Rc.on(CR[1][FX,FEX,VX,OX])), 
      (PSR[rA], PSR[rB], FPSCR[VE]))

   INS(psq_lux, (frD), (rA, rB), (qw, qi), (opcd == 4, xo3 == 38, !_31), (rA == 0), USER, "Paired Single Load with Update Indexed",
      (PSR[rD], GPR[rA]), 
      ( w.eq(0, GQR[i].MEM(GPR[rA] + GPR[rB], 2)),
        w.eq(1, GQR[i].MEM(GPR[rA] + GPR[rB], 1)) ))

   ins_add;
   ins_mtfsfi;
   ins_ps_add;
   ins_psq_lux;
   printf("LOL");
   //*/

   // Verify instruction fields
#define FLD(x, y, z, ...) \
   static_assert(z >= y, "Field " #x " z < y"); \
   Instruction ins_##x(make_bitmask<31-z,31-y,uint32_t>()); \
   uint32_t insv_##x = make_bitmask<0, z - y, uint32_t>(); \
   if (ins_##x.x != insv_##x) { \
      printf("%s %08x %08x\n", #x, ins_##x.x, insv_##x); \
   }
#define MRKR(...)
#include "instructionfields.inl"
#undef FLD
#undef MRKR
};

#undef INS
#undef INSA