//! \file
/*
**  Copyright (C) - Triton
**
**  This program is under the terms of the Apache License 2.0.
*/

#include <algorithm>
#include <cstring>

#include <triton/architecture.hpp>
#include <triton/coreUtils.hpp>
#include <triton/cpuSize.hpp>
#include <triton/exceptions.hpp>
#include <triton/immediate.hpp>
#include <triton/x86Cpu.hpp>



namespace triton {
  namespace arch {
    namespace x86 {

      x86Cpu::x86Cpu(triton::callbacks::Callbacks* callbacks) : x86Specifications(ARCH_X86) {
        this->callbacks = callbacks;
        this->handle    = 0;

        this->clear();
        this->disassInit();
      }


      x86Cpu::x86Cpu(const x86Cpu& other) : x86Specifications(ARCH_X86) {
        this->copy(other);
      }


      x86Cpu::~x86Cpu() {
        this->memory.clear();
        if (this->handle) {
          triton::extlibs::capstone::cs_close(&this->handle);
        }
      }


      void x86Cpu::disassInit(void) {
        if (this->handle) {
          triton::extlibs::capstone::cs_close(&this->handle);
        }

        if (triton::extlibs::capstone::cs_open(triton::extlibs::capstone::CS_ARCH_X86, triton::extlibs::capstone::CS_MODE_32, &this->handle) != triton::extlibs::capstone::CS_ERR_OK)
          throw triton::exceptions::Disassembly("x86Cpu::disassInit(): Cannot open capstone.");

        triton::extlibs::capstone::cs_option(this->handle, triton::extlibs::capstone::CS_OPT_DETAIL, triton::extlibs::capstone::CS_OPT_ON);
        triton::extlibs::capstone::cs_option(this->handle, triton::extlibs::capstone::CS_OPT_SYNTAX, triton::extlibs::capstone::CS_OPT_SYNTAX_INTEL);
      }


      void x86Cpu::copy(const x86Cpu& other) {
        this->callbacks = other.callbacks;
        this->memory    = other.memory;

        std::memcpy(this->eax,     other.eax,    sizeof(this->eax));
        std::memcpy(this->ebx,     other.ebx,    sizeof(this->ebx));
        std::memcpy(this->ecx,     other.ecx,    sizeof(this->ecx));
        std::memcpy(this->edx,     other.edx,    sizeof(this->edx));
        std::memcpy(this->edi,     other.edi,    sizeof(this->edi));
        std::memcpy(this->esi,     other.esi,    sizeof(this->esi));
        std::memcpy(this->esp,     other.esp,    sizeof(this->esp));
        std::memcpy(this->ebp,     other.ebp,    sizeof(this->ebp));
        std::memcpy(this->eip,     other.eip,    sizeof(this->eip));
        std::memcpy(this->eflags,  other.eflags, sizeof(this->eflags));
        std::memcpy(this->mm0,     other.mm0,    sizeof(this->mm0));
        std::memcpy(this->mm1,     other.mm1,    sizeof(this->mm1));
        std::memcpy(this->mm2,     other.mm2,    sizeof(this->mm2));
        std::memcpy(this->mm3,     other.mm3,    sizeof(this->mm3));
        std::memcpy(this->mm4,     other.mm4,    sizeof(this->mm4));
        std::memcpy(this->mm5,     other.mm5,    sizeof(this->mm5));
        std::memcpy(this->mm6,     other.mm6,    sizeof(this->mm6));
        std::memcpy(this->mm7,     other.mm7,    sizeof(this->mm7));
        std::memcpy(this->ymm0,    other.ymm0,   sizeof(this->ymm0));
        std::memcpy(this->ymm1,    other.ymm1,   sizeof(this->ymm1));
        std::memcpy(this->ymm2,    other.ymm2,   sizeof(this->ymm2));
        std::memcpy(this->ymm3,    other.ymm3,   sizeof(this->ymm3));
        std::memcpy(this->ymm4,    other.ymm4,   sizeof(this->ymm4));
        std::memcpy(this->ymm5,    other.ymm5,   sizeof(this->ymm5));
        std::memcpy(this->ymm6,    other.ymm6,   sizeof(this->ymm6));
        std::memcpy(this->ymm7,    other.ymm7,   sizeof(this->ymm7));
        std::memcpy(this->mxcsr,   other.mxcsr,  sizeof(this->mxcsr));
        std::memcpy(this->cr0,     other.cr0,    sizeof(this->cr0));
        std::memcpy(this->cr1,     other.cr1,    sizeof(this->cr1));
        std::memcpy(this->cr2,     other.cr2,    sizeof(this->cr2));
        std::memcpy(this->cr3,     other.cr3,    sizeof(this->cr3));
        std::memcpy(this->cr4,     other.cr4,    sizeof(this->cr4));
        std::memcpy(this->cr5,     other.cr5,    sizeof(this->cr5));
        std::memcpy(this->cr6,     other.cr6,    sizeof(this->cr6));
        std::memcpy(this->cr7,     other.cr7,    sizeof(this->cr7));
        std::memcpy(this->cr8,     other.cr8,    sizeof(this->cr8));
        std::memcpy(this->cr9,     other.cr9,    sizeof(this->cr9));
        std::memcpy(this->cr10,    other.cr10,   sizeof(this->cr10));
        std::memcpy(this->cr11,    other.cr11,   sizeof(this->cr11));
        std::memcpy(this->cr12,    other.cr12,   sizeof(this->cr12));
        std::memcpy(this->cr13,    other.cr13,   sizeof(this->cr13));
        std::memcpy(this->cr14,    other.cr14,   sizeof(this->cr14));
        std::memcpy(this->cr15,    other.cr15,   sizeof(this->cr15));
        std::memcpy(this->cs,      other.cs,     sizeof(this->cs));
        std::memcpy(this->ds,      other.ds,     sizeof(this->ds));
        std::memcpy(this->es,      other.es,     sizeof(this->es));
        std::memcpy(this->fs,      other.fs,     sizeof(this->fs));
        std::memcpy(this->gs,      other.gs,     sizeof(this->gs));
        std::memcpy(this->ss,      other.ss,     sizeof(this->ss));
        std::memcpy(this->dr0,     other.dr0,    sizeof(this->dr0));
        std::memcpy(this->dr1,     other.dr1,    sizeof(this->dr1));
        std::memcpy(this->dr2,     other.dr2,    sizeof(this->dr2));
        std::memcpy(this->dr3,     other.dr3,    sizeof(this->dr3));
        std::memcpy(this->dr6,     other.dr6,    sizeof(this->dr6));
        std::memcpy(this->dr7,     other.dr7,    sizeof(this->dr7));
      }


      void x86Cpu::clear(void) {
        /* Clear memory */
        this->memory.clear();

        /* Clear registers */
        std::memset(this->eax,     0x00, sizeof(this->eax));
        std::memset(this->ebx,     0x00, sizeof(this->ebx));
        std::memset(this->ecx,     0x00, sizeof(this->ecx));
        std::memset(this->edx,     0x00, sizeof(this->edx));
        std::memset(this->edi,     0x00, sizeof(this->edi));
        std::memset(this->esi,     0x00, sizeof(this->esi));
        std::memset(this->esp,     0x00, sizeof(this->esp));
        std::memset(this->ebp,     0x00, sizeof(this->ebp));
        std::memset(this->eip,     0x00, sizeof(this->eip));
        std::memset(this->eflags,  0x00, sizeof(this->eflags));
        std::memset(this->mm0,     0x00, sizeof(this->mm0));
        std::memset(this->mm1,     0x00, sizeof(this->mm1));
        std::memset(this->mm2,     0x00, sizeof(this->mm2));
        std::memset(this->mm3,     0x00, sizeof(this->mm3));
        std::memset(this->mm4,     0x00, sizeof(this->mm4));
        std::memset(this->mm5,     0x00, sizeof(this->mm5));
        std::memset(this->mm6,     0x00, sizeof(this->mm6));
        std::memset(this->mm7,     0x00, sizeof(this->mm7));
        std::memset(this->ymm0,    0x00, sizeof(this->ymm0));
        std::memset(this->ymm1,    0x00, sizeof(this->ymm1));
        std::memset(this->ymm2,    0x00, sizeof(this->ymm2));
        std::memset(this->ymm3,    0x00, sizeof(this->ymm3));
        std::memset(this->ymm4,    0x00, sizeof(this->ymm4));
        std::memset(this->ymm5,    0x00, sizeof(this->ymm5));
        std::memset(this->ymm6,    0x00, sizeof(this->ymm6));
        std::memset(this->ymm7,    0x00, sizeof(this->ymm7));
        std::memset(this->mxcsr,   0x00, sizeof(this->mxcsr));
        std::memset(this->cr0,     0x00, sizeof(this->cr0));
        std::memset(this->cr1,     0x00, sizeof(this->cr1));
        std::memset(this->cr2,     0x00, sizeof(this->cr2));
        std::memset(this->cr3,     0x00, sizeof(this->cr3));
        std::memset(this->cr4,     0x00, sizeof(this->cr4));
        std::memset(this->cr5,     0x00, sizeof(this->cr5));
        std::memset(this->cr6,     0x00, sizeof(this->cr6));
        std::memset(this->cr7,     0x00, sizeof(this->cr7));
        std::memset(this->cr8,     0x00, sizeof(this->cr8));
        std::memset(this->cr9,     0x00, sizeof(this->cr9));
        std::memset(this->cr10,    0x00, sizeof(this->cr10));
        std::memset(this->cr11,    0x00, sizeof(this->cr11));
        std::memset(this->cr12,    0x00, sizeof(this->cr12));
        std::memset(this->cr13,    0x00, sizeof(this->cr13));
        std::memset(this->cr14,    0x00, sizeof(this->cr14));
        std::memset(this->cr15,    0x00, sizeof(this->cr15));
        std::memset(this->cs,      0x00, sizeof(this->cs));
        std::memset(this->ds,      0x00, sizeof(this->ds));
        std::memset(this->es,      0x00, sizeof(this->es));
        std::memset(this->fs,      0x00, sizeof(this->fs));
        std::memset(this->gs,      0x00, sizeof(this->gs));
        std::memset(this->ss,      0x00, sizeof(this->ss));
        std::memset(this->dr0,     0x00, sizeof(this->dr0));
        std::memset(this->dr1,     0x00, sizeof(this->dr1));
        std::memset(this->dr2,     0x00, sizeof(this->dr2));
        std::memset(this->dr3,     0x00, sizeof(this->dr3));
        std::memset(this->dr6,     0x00, sizeof(this->dr6));
        std::memset(this->dr7,     0x00, sizeof(this->dr7));
      }


      x86Cpu& x86Cpu::operator=(const x86Cpu& other) {
        this->copy(other);
        return *this;
      }


      triton::arch::endianness_e x86Cpu::getEndianness(void) const {
        return triton::arch::LE_ENDIANNESS;
      }


      bool x86Cpu::isFlag(triton::arch::register_e regId) const {
        return ((regId >= triton::arch::ID_REG_X86_AC && regId <= triton::arch::ID_REG_X86_FZ) ? true : false);
      }


      bool x86Cpu::isRegister(triton::arch::register_e regId) const {
        return (
          this->isGPR(regId)      ||
          this->isMMX(regId)      ||
          this->isSSE(regId)      ||
          this->isAVX256(regId)   ||
          this->isControl(regId)  ||
          this->isDebug(regId)    ||
          this->isSegment(regId)
        );
      }


      bool x86Cpu::isRegisterValid(triton::arch::register_e regId) const {
        return (this->isFlag(regId) || this->isRegister(regId));
      }


      bool x86Cpu::isGPR(triton::arch::register_e regId) const {
        return ((regId >= triton::arch::ID_REG_X86_EAX && regId <= triton::arch::ID_REG_X86_EFLAGS) ? true : false);
      }


      bool x86Cpu::isMMX(triton::arch::register_e regId) const {
        return ((regId >= triton::arch::ID_REG_X86_MM0 && regId <= triton::arch::ID_REG_X86_MM7) ? true : false);
      }


      bool x86Cpu::isSSE(triton::arch::register_e regId) const {
        return ((regId >= triton::arch::ID_REG_X86_MXCSR && regId <= triton::arch::ID_REG_X86_XMM7) ? true : false);
      }


      bool x86Cpu::isAVX256(triton::arch::register_e regId) const {
        return ((regId >= triton::arch::ID_REG_X86_YMM0 && regId <= triton::arch::ID_REG_X86_YMM7) ? true : false);
      }


      bool x86Cpu::isControl(triton::arch::register_e regId) const {
        return ((regId >= triton::arch::ID_REG_X86_CR0 && regId <= triton::arch::ID_REG_X86_CR15) ? true : false);
      }


      bool x86Cpu::isDebug(triton::arch::register_e regId) const {
        return ((regId >= triton::arch::ID_REG_X86_DR0 && regId <= triton::arch::ID_REG_X86_DR7) ? true : false);
      }


      bool x86Cpu::isSegment(triton::arch::register_e regId) const {
        return ((regId >= triton::arch::ID_REG_X86_CS && regId <= triton::arch::ID_REG_X86_SS) ? true : false);
      }


      triton::uint32 x86Cpu::numberOfRegisters(void) const {
        return triton::arch::ID_REG_LAST_ITEM;
      }


      triton::uint32 x86Cpu::gprSize(void) const {
        return triton::size::dword;
      }


      triton::uint32 x86Cpu::gprBitSize(void) const {
        return triton::bitsize::dword;
      }



      const std::unordered_map<triton::arch::register_e, const triton::arch::Register>& x86Cpu::getAllRegisters(void) const {
        return this->id2reg;
      }


      std::set<const triton::arch::Register*> x86Cpu::getParentRegisters(void) const {
        std::set<const triton::arch::Register*> ret;

        for (const auto& kv: this->id2reg) {
          auto regId = kv.first;
          const auto& reg = kv.second;

          /* Add GPR */
          if (reg.getSize() == this->gprSize())
            ret.insert(&reg);

          /* Add Flags */
          else if (this->isFlag(regId))
            ret.insert(&reg);

          /* Add MMX */
          else if (this->isMMX(regId))
            ret.insert(&reg);

          /* Add SSE */
          else if (this->isSSE(regId))
            ret.insert(&reg);

          /* Add AVX-256 */
          else if (this->isAVX256(regId))
            ret.insert(&reg);

          /* Add Control */
          else if (this->isControl(regId))
            ret.insert(&reg);

          /* Add Debug */
          else if (this->isDebug(regId))
            ret.insert(&reg);

          /* Add Segment */
          else if (this->isSegment(regId))
            ret.insert(&reg);
        }

        return ret;
      }


      const triton::arch::Register& x86Cpu::getRegister(triton::arch::register_e id) const {
        try {
          return this->id2reg.at(id);
        } catch (const std::out_of_range&) {
          throw triton::exceptions::Cpu("x86Cpu::getRegister(): Invalid register for this architecture.");
        }
      }


      const triton::arch::Register& x86Cpu::getRegister(const std::string& name) const {
        std::string lower = name;
        std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c){ return std::tolower(c); });
        try {
          return this->getRegister(this->name2id.at(lower));
        } catch (const std::out_of_range&) {
          throw triton::exceptions::Cpu("x86Cpu::getRegister(): Invalid register for this architecture.");
        }
      }


      const triton::arch::Register& x86Cpu::getParentRegister(const triton::arch::Register& reg) const {
        return this->getRegister(reg.getParent());
      }


      const triton::arch::Register& x86Cpu::getParentRegister(triton::arch::register_e id) const {
        return this->getParentRegister(this->getRegister(id));
      }


      const triton::arch::Register& x86Cpu::getProgramCounter(void) const {
        return this->getRegister(this->pcId);
      }


      const triton::arch::Register& x86Cpu::getStackPointer(void) const {
        return this->getRegister(this->spId);
      }


      void x86Cpu::disassembly(triton::arch::Instruction& inst) {
        triton::extlibs::capstone::cs_insn* insn;
        triton::usize count = 0;

        /* Check if the opcode and opcode' size are defined */
        if (inst.getOpcode() == nullptr || inst.getSize() == 0)
          throw triton::exceptions::Disassembly("x86Cpu::disassembly(): Opcode and opcodeSize must be definied.");

        /* Clear instructicon's operands if alredy defined */
        inst.operands.clear();

        /* Update instruction address if undefined */
        if (!inst.getAddress()) {
          inst.setAddress(this->getConcreteRegisterValue(this->getProgramCounter()).convert_to<triton::uint64>());
        }

        /* Let's disass and build our operands */
        count = triton::extlibs::capstone::cs_disasm(this->handle, inst.getOpcode(), inst.getSize(), inst.getAddress(), 0, &insn);
        if (count > 0) {
          /* Detail information */
          triton::extlibs::capstone::cs_detail* detail = insn->detail;

          /* Init the disassembly */
          std::stringstream str;

          str << insn[0].mnemonic;
          if (detail->x86.op_count)
            str << " " <<  insn[0].op_str;

          inst.setDisassembly(str.str());

          /* Refine the size */
          inst.setSize(insn[0].size);

          /* Init the instruction's type */
          inst.setType(this->capstoneInstructionToTritonInstruction(insn[0].id));

          /* Init the instruction's prefix */
          inst.setPrefix(this->capstonePrefixToTritonPrefix(detail->x86.prefix[0]));

          /* Init operands */
          for (triton::uint32 n = 0; n < detail->x86.op_count; n++) {
            triton::extlibs::capstone::cs_x86_op* op = &(detail->x86.operands[n]);
            switch(op->type) {

              case triton::extlibs::capstone::X86_OP_IMM:
                inst.operands.push_back(triton::arch::OperandWrapper(triton::arch::Immediate(op->imm, op->size)));
                break;

              case triton::extlibs::capstone::X86_OP_MEM: {
                triton::arch::MemoryAccess mem;

                /* Set the size of the memory access */
                mem.setBits(((op->size * triton::bitsize::byte) - 1), 0);

                /* LEA if exists */
                const triton::arch::Register segment(*this, this->capstoneRegisterToTritonRegister(op->mem.segment));
                const triton::arch::Register base(*this, this->capstoneRegisterToTritonRegister(op->mem.base));
                const triton::arch::Register index(*this, this->capstoneRegisterToTritonRegister(op->mem.index));

                triton::uint32 immsize = (
                  this->isRegisterValid(base.getId()) ? base.getSize() :
                  this->isRegisterValid(index.getId()) ? index.getSize() :
                  this->gprSize()
                );

                triton::arch::Immediate disp(op->mem.disp, immsize);
                triton::arch::Immediate scale(op->mem.scale, immsize);

                /* Specify that LEA contains a PC relative */
                if (base.getId() == this->pcId)
                  mem.setPcRelative(inst.getNextAddress());

                mem.setSegmentRegister(segment);
                mem.setBaseRegister(base);
                mem.setIndexRegister(index);
                mem.setDisplacement(disp);
                mem.setScale(scale);

                inst.operands.push_back(triton::arch::OperandWrapper(mem));
                break;
              }

              case triton::extlibs::capstone::X86_OP_REG:
                inst.operands.push_back(triton::arch::OperandWrapper(triton::arch::Register(*this, this->capstoneRegisterToTritonRegister(op->reg))));
                break;

              default:
                break;
            }

          }
          /* Set branch */
          if (detail->groups_count > 0) {
            for (triton::uint32 n = 0; n < detail->groups_count; n++) {
              if (detail->groups[n] == triton::extlibs::capstone::X86_GRP_JUMP)
                inst.setBranch(true);
              if (detail->groups[n] == triton::extlibs::capstone::X86_GRP_JUMP ||
                  detail->groups[n] == triton::extlibs::capstone::X86_GRP_CALL ||
                  detail->groups[n] == triton::extlibs::capstone::X86_GRP_RET)
                inst.setControlFlow(true);
            }
          }
          triton::extlibs::capstone::cs_free(insn, count);
        }
        else
          throw triton::exceptions::Disassembly("x86Cpu::disassembly(): Failed to disassemble the given code.");
      }


      triton::uint8 x86Cpu::getConcreteMemoryValue(triton::uint64 addr, bool execCallbacks) const {
        if (execCallbacks && this->callbacks)
          this->callbacks->processCallbacks(triton::callbacks::GET_CONCRETE_MEMORY_VALUE, MemoryAccess(addr, triton::size::byte));

        auto it = this->memory.find(addr);
        if (it == this->memory.end())
          return 0x00;

        return it->second;
      }


      triton::uint512 x86Cpu::getConcreteMemoryValue(const triton::arch::MemoryAccess& mem, bool execCallbacks) const {
        triton::uint512 ret = 0;
        triton::uint64 addr = 0;
        triton::uint32 size = 0;

        if (execCallbacks && this->callbacks)
          this->callbacks->processCallbacks(triton::callbacks::GET_CONCRETE_MEMORY_VALUE, mem);

        addr = mem.getAddress();
        size = mem.getSize();

        if (size == 0 || size > triton::size::dqqword)
          throw triton::exceptions::Cpu("x86Cpu::getConcreteMemoryValue(): Invalid size memory.");

        for (triton::sint32 i = size-1; i >= 0; i--)
          ret = ((ret << triton::bitsize::byte) | this->getConcreteMemoryValue(addr+i, false));

        return ret;
      }


      std::vector<triton::uint8> x86Cpu::getConcreteMemoryAreaValue(triton::uint64 baseAddr, triton::usize size, bool execCallbacks) const {
        std::vector<triton::uint8> area;

        for (triton::usize index = 0; index < size; index++)
          area.push_back(this->getConcreteMemoryValue(baseAddr+index));

        return area;
      }


      triton::uint512 x86Cpu::getConcreteRegisterValue(const triton::arch::Register& reg, bool execCallbacks) const {
        triton::uint512 value = 0;

        if (execCallbacks && this->callbacks)
          this->callbacks->processCallbacks(triton::callbacks::GET_CONCRETE_REGISTER_VALUE, reg);

        switch (reg.getId()) {
          case triton::arch::ID_REG_X86_EAX: return (*((triton::uint32*)(this->eax)));
          case triton::arch::ID_REG_X86_AX:  return (*((triton::uint16*)(this->eax)));
          case triton::arch::ID_REG_X86_AH:  return (*((triton::uint8*)(this->eax+1)));
          case triton::arch::ID_REG_X86_AL:  return (*((triton::uint8*)(this->eax)));

          case triton::arch::ID_REG_X86_EBX: return (*((triton::uint32*)(this->ebx)));
          case triton::arch::ID_REG_X86_BX:  return (*((triton::uint16*)(this->ebx)));
          case triton::arch::ID_REG_X86_BH:  return (*((triton::uint8*)(this->ebx+1)));
          case triton::arch::ID_REG_X86_BL:  return (*((triton::uint8*)(this->ebx)));

          case triton::arch::ID_REG_X86_ECX: return (*((triton::uint32*)(this->ecx)));
          case triton::arch::ID_REG_X86_CX:  return (*((triton::uint16*)(this->ecx)));
          case triton::arch::ID_REG_X86_CH:  return (*((triton::uint8*)(this->ecx+1)));
          case triton::arch::ID_REG_X86_CL:  return (*((triton::uint8*)(this->ecx)));

          case triton::arch::ID_REG_X86_EDX: return (*((triton::uint32*)(this->edx)));
          case triton::arch::ID_REG_X86_DX:  return (*((triton::uint16*)(this->edx)));
          case triton::arch::ID_REG_X86_DH:  return (*((triton::uint8*)(this->edx+1)));
          case triton::arch::ID_REG_X86_DL:  return (*((triton::uint8*)(this->edx)));

          case triton::arch::ID_REG_X86_EDI: return (*((triton::uint32*)(this->edi)));
          case triton::arch::ID_REG_X86_DI:  return (*((triton::uint16*)(this->edi)));
          case triton::arch::ID_REG_X86_DIL: return (*((triton::uint8*)(this->edi)));

          case triton::arch::ID_REG_X86_ESI: return (*((triton::uint32*)(this->esi)));
          case triton::arch::ID_REG_X86_SI:  return (*((triton::uint16*)(this->esi)));
          case triton::arch::ID_REG_X86_SIL: return (*((triton::uint8*)(this->esi)));

          case triton::arch::ID_REG_X86_ESP: return (*((triton::uint32*)(this->esp)));
          case triton::arch::ID_REG_X86_SP:  return (*((triton::uint16*)(this->esp)));
          case triton::arch::ID_REG_X86_SPL: return (*((triton::uint8*)(this->esp)));

          case triton::arch::ID_REG_X86_EBP: return (*((triton::uint32*)(this->ebp)));
          case triton::arch::ID_REG_X86_BP:  return (*((triton::uint16*)(this->ebp)));
          case triton::arch::ID_REG_X86_BPL: return (*((triton::uint8*)(this->ebp)));

          case triton::arch::ID_REG_X86_EIP: return (*((triton::uint32*)(this->eip)));
          case triton::arch::ID_REG_X86_IP:  return (*((triton::uint16*)(this->eip)));

          case triton::arch::ID_REG_X86_EFLAGS: return (*((triton::uint32*)(this->eflags)));

          case triton::arch::ID_REG_X86_MM0:  return (*((triton::uint64*)(this->mm0)));
          case triton::arch::ID_REG_X86_MM1:  return (*((triton::uint64*)(this->mm1)));
          case triton::arch::ID_REG_X86_MM2:  return (*((triton::uint64*)(this->mm2)));
          case triton::arch::ID_REG_X86_MM3:  return (*((triton::uint64*)(this->mm3)));
          case triton::arch::ID_REG_X86_MM4:  return (*((triton::uint64*)(this->mm4)));
          case triton::arch::ID_REG_X86_MM5:  return (*((triton::uint64*)(this->mm5)));
          case triton::arch::ID_REG_X86_MM6:  return (*((triton::uint64*)(this->mm6)));
          case triton::arch::ID_REG_X86_MM7:  return (*((triton::uint64*)(this->mm7)));

          case triton::arch::ID_REG_X86_XMM0: value = triton::utils::fromBufferToUint<triton::uint128>(this->ymm0); return value;
          case triton::arch::ID_REG_X86_XMM1: value = triton::utils::fromBufferToUint<triton::uint128>(this->ymm1); return value;
          case triton::arch::ID_REG_X86_XMM2: value = triton::utils::fromBufferToUint<triton::uint128>(this->ymm2); return value;
          case triton::arch::ID_REG_X86_XMM3: value = triton::utils::fromBufferToUint<triton::uint128>(this->ymm3); return value;
          case triton::arch::ID_REG_X86_XMM4: value = triton::utils::fromBufferToUint<triton::uint128>(this->ymm4); return value;
          case triton::arch::ID_REG_X86_XMM5: value = triton::utils::fromBufferToUint<triton::uint128>(this->ymm5); return value;
          case triton::arch::ID_REG_X86_XMM6: value = triton::utils::fromBufferToUint<triton::uint128>(this->ymm6); return value;
          case triton::arch::ID_REG_X86_XMM7: value = triton::utils::fromBufferToUint<triton::uint128>(this->ymm7); return value;

          case triton::arch::ID_REG_X86_YMM0: value = triton::utils::fromBufferToUint<triton::uint256>(this->ymm0); return value;
          case triton::arch::ID_REG_X86_YMM1: value = triton::utils::fromBufferToUint<triton::uint256>(this->ymm1); return value;
          case triton::arch::ID_REG_X86_YMM2: value = triton::utils::fromBufferToUint<triton::uint256>(this->ymm2); return value;
          case triton::arch::ID_REG_X86_YMM3: value = triton::utils::fromBufferToUint<triton::uint256>(this->ymm3); return value;
          case triton::arch::ID_REG_X86_YMM4: value = triton::utils::fromBufferToUint<triton::uint256>(this->ymm4); return value;
          case triton::arch::ID_REG_X86_YMM5: value = triton::utils::fromBufferToUint<triton::uint256>(this->ymm5); return value;
          case triton::arch::ID_REG_X86_YMM6: value = triton::utils::fromBufferToUint<triton::uint256>(this->ymm6); return value;
          case triton::arch::ID_REG_X86_YMM7: value = triton::utils::fromBufferToUint<triton::uint256>(this->ymm7); return value;

          case triton::arch::ID_REG_X86_MXCSR: return (*((triton::uint32*)(this->mxcsr)));

          case triton::arch::ID_REG_X86_CR0:  return (*((triton::uint32*)(this->cr0)));
          case triton::arch::ID_REG_X86_CR1:  return (*((triton::uint32*)(this->cr1)));
          case triton::arch::ID_REG_X86_CR2:  return (*((triton::uint32*)(this->cr2)));
          case triton::arch::ID_REG_X86_CR3:  return (*((triton::uint32*)(this->cr3)));
          case triton::arch::ID_REG_X86_CR4:  return (*((triton::uint32*)(this->cr4)));
          case triton::arch::ID_REG_X86_CR5:  return (*((triton::uint32*)(this->cr5)));
          case triton::arch::ID_REG_X86_CR6:  return (*((triton::uint32*)(this->cr6)));
          case triton::arch::ID_REG_X86_CR7:  return (*((triton::uint32*)(this->cr7)));
          case triton::arch::ID_REG_X86_CR8:  return (*((triton::uint32*)(this->cr8)));
          case triton::arch::ID_REG_X86_CR9:  return (*((triton::uint32*)(this->cr9)));
          case triton::arch::ID_REG_X86_CR10: return (*((triton::uint32*)(this->cr10)));
          case triton::arch::ID_REG_X86_CR11: return (*((triton::uint32*)(this->cr11)));
          case triton::arch::ID_REG_X86_CR12: return (*((triton::uint32*)(this->cr12)));
          case triton::arch::ID_REG_X86_CR13: return (*((triton::uint32*)(this->cr13)));
          case triton::arch::ID_REG_X86_CR14: return (*((triton::uint32*)(this->cr14)));
          case triton::arch::ID_REG_X86_CR15: return (*((triton::uint32*)(this->cr15)));

          case triton::arch::ID_REG_X86_DR0: return (*((triton::uint32*)(this->dr0)));
          case triton::arch::ID_REG_X86_DR1: return (*((triton::uint32*)(this->dr1)));
          case triton::arch::ID_REG_X86_DR2: return (*((triton::uint32*)(this->dr2)));
          case triton::arch::ID_REG_X86_DR3: return (*((triton::uint32*)(this->dr3)));
          case triton::arch::ID_REG_X86_DR6: return (*((triton::uint32*)(this->dr6)));
          case triton::arch::ID_REG_X86_DR7: return (*((triton::uint32*)(this->dr7)));

          case triton::arch::ID_REG_X86_IE:  return (((*((triton::uint32*)(this->mxcsr))) >> 0) & 1);
          case triton::arch::ID_REG_X86_DE:  return (((*((triton::uint32*)(this->mxcsr))) >> 1) & 1);
          case triton::arch::ID_REG_X86_ZE:  return (((*((triton::uint32*)(this->mxcsr))) >> 2) & 1);
          case triton::arch::ID_REG_X86_OE:  return (((*((triton::uint32*)(this->mxcsr))) >> 3) & 1);
          case triton::arch::ID_REG_X86_UE:  return (((*((triton::uint32*)(this->mxcsr))) >> 4) & 1);
          case triton::arch::ID_REG_X86_PE:  return (((*((triton::uint32*)(this->mxcsr))) >> 5) & 1);
          case triton::arch::ID_REG_X86_DAZ: return (((*((triton::uint32*)(this->mxcsr))) >> 6) & 1);
          case triton::arch::ID_REG_X86_IM:  return (((*((triton::uint32*)(this->mxcsr))) >> 7) & 1);
          case triton::arch::ID_REG_X86_DM:  return (((*((triton::uint32*)(this->mxcsr))) >> 8) & 1);
          case triton::arch::ID_REG_X86_ZM:  return (((*((triton::uint32*)(this->mxcsr))) >> 9) & 1);
          case triton::arch::ID_REG_X86_OM:  return (((*((triton::uint32*)(this->mxcsr))) >> 10) & 1);
          case triton::arch::ID_REG_X86_UM:  return (((*((triton::uint32*)(this->mxcsr))) >> 11) & 1);
          case triton::arch::ID_REG_X86_PM:  return (((*((triton::uint32*)(this->mxcsr))) >> 12) & 1);
          case triton::arch::ID_REG_X86_RL:  return (((*((triton::uint32*)(this->mxcsr))) >> 13) & 1);
          case triton::arch::ID_REG_X86_RH:  return (((*((triton::uint32*)(this->mxcsr))) >> 14) & 1);
          case triton::arch::ID_REG_X86_FZ:  return (((*((triton::uint32*)(this->mxcsr))) >> 15) & 1);

          case triton::arch::ID_REG_X86_CF:  return (((*((triton::uint64*)(this->eflags))) >> 0) & 1);
          case triton::arch::ID_REG_X86_PF:  return (((*((triton::uint64*)(this->eflags))) >> 2) & 1);
          case triton::arch::ID_REG_X86_AF:  return (((*((triton::uint64*)(this->eflags))) >> 4) & 1);
          case triton::arch::ID_REG_X86_ZF:  return (((*((triton::uint64*)(this->eflags))) >> 6) & 1);
          case triton::arch::ID_REG_X86_SF:  return (((*((triton::uint64*)(this->eflags))) >> 7) & 1);
          case triton::arch::ID_REG_X86_TF:  return (((*((triton::uint64*)(this->eflags))) >> 8) & 1);
          case triton::arch::ID_REG_X86_IF:  return (((*((triton::uint64*)(this->eflags))) >> 9) & 1);
          case triton::arch::ID_REG_X86_DF:  return (((*((triton::uint64*)(this->eflags))) >> 10) & 1);
          case triton::arch::ID_REG_X86_OF:  return (((*((triton::uint64*)(this->eflags))) >> 11) & 1);
          case triton::arch::ID_REG_X86_NT:  return (((*((triton::uint64*)(this->eflags))) >> 14) & 1);
          case triton::arch::ID_REG_X86_RF:  return (((*((triton::uint64*)(this->eflags))) >> 16) & 1);
          case triton::arch::ID_REG_X86_VM:  return (((*((triton::uint64*)(this->eflags))) >> 17) & 1);
          case triton::arch::ID_REG_X86_AC:  return (((*((triton::uint64*)(this->eflags))) >> 18) & 1);
          case triton::arch::ID_REG_X86_VIF: return (((*((triton::uint64*)(this->eflags))) >> 19) & 1);
          case triton::arch::ID_REG_X86_VIP: return (((*((triton::uint64*)(this->eflags))) >> 20) & 1);
          case triton::arch::ID_REG_X86_ID:  return (((*((triton::uint64*)(this->eflags))) >> 21) & 1);

          case triton::arch::ID_REG_X86_CS: return (*((triton::uint32*)(this->cs)));
          case triton::arch::ID_REG_X86_DS: return (*((triton::uint32*)(this->ds)));
          case triton::arch::ID_REG_X86_ES: return (*((triton::uint32*)(this->es)));
          case triton::arch::ID_REG_X86_FS: return (*((triton::uint32*)(this->fs)));
          case triton::arch::ID_REG_X86_GS: return (*((triton::uint32*)(this->gs)));
          case triton::arch::ID_REG_X86_SS: return (*((triton::uint32*)(this->ss)));

          default:
            throw triton::exceptions::Cpu("x86Cpu::getConcreteRegisterValue(): Invalid register.");
        }

        return value;
      }


      void x86Cpu::setConcreteMemoryValue(triton::uint64 addr, triton::uint8 value) {
        if (this->callbacks)
          this->callbacks->processCallbacks(triton::callbacks::SET_CONCRETE_MEMORY_VALUE, MemoryAccess(addr, triton::size::byte), value);
        this->memory[addr] = value;
      }


      void x86Cpu::setConcreteMemoryValue(const triton::arch::MemoryAccess& mem, const triton::uint512& value) {
        triton::uint64 addr = mem.getAddress();
        triton::uint32 size = mem.getSize();
        triton::uint512 cv  = value;

        if (cv > mem.getMaxValue())
          throw triton::exceptions::Register("x86Cpu::setConcreteMemoryValue(): You cannot set this concrete value (too big) to this memory access.");

        if (size == 0 || size > triton::size::dqqword)
          throw triton::exceptions::Cpu("x86Cpu::setConcreteMemoryValue(): Invalid size memory.");

        if (this->callbacks)
          this->callbacks->processCallbacks(triton::callbacks::SET_CONCRETE_MEMORY_VALUE, mem, value);

        for (triton::uint32 i = 0; i < size; i++) {
          this->memory[addr+i] = (cv & 0xff).convert_to<triton::uint8>();
          cv >>= 8;
        }
      }


      void x86Cpu::setConcreteMemoryAreaValue(triton::uint64 baseAddr, const std::vector<triton::uint8>& values) {
        this->memory.reserve(values.size() + this->memory.size());
        for (triton::usize index = 0; index < values.size(); index++) {
          this->setConcreteMemoryValue(baseAddr+index, values[index]);
        }
      }


      void x86Cpu::setConcreteMemoryAreaValue(triton::uint64 baseAddr, const triton::uint8* area, triton::usize size) {
        this->memory.reserve(size + this->memory.size());
        for (triton::usize index = 0; index < size; index++) {
          this->setConcreteMemoryValue(baseAddr+index, area[index]);
        }
      }


      void x86Cpu::setConcreteRegisterValue(const triton::arch::Register& reg, const triton::uint512& value) {
        if (value > reg.getMaxValue())
          throw triton::exceptions::Register("x86Cpu::setConcreteRegisterValue(): You cannot set this concrete value (too big) to this register.");

        if (this->callbacks)
          this->callbacks->processCallbacks(triton::callbacks::SET_CONCRETE_REGISTER_VALUE, reg, value);

        switch (reg.getId()) {
          case triton::arch::ID_REG_X86_EAX: (*((triton::uint32*)(this->eax)))  = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_AX:  (*((triton::uint16*)(this->eax)))  = value.convert_to<triton::uint16>(); break;
          case triton::arch::ID_REG_X86_AH:  (*((triton::uint8*)(this->eax+1))) = value.convert_to<triton::uint8>(); break;
          case triton::arch::ID_REG_X86_AL:  (*((triton::uint8*)(this->eax)))   = value.convert_to<triton::uint8>(); break;

          case triton::arch::ID_REG_X86_EBX: (*((triton::uint32*)(this->ebx)))  = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_BX:  (*((triton::uint16*)(this->ebx)))  = value.convert_to<triton::uint16>(); break;
          case triton::arch::ID_REG_X86_BH:  (*((triton::uint8*)(this->ebx+1))) = value.convert_to<triton::uint8>(); break;
          case triton::arch::ID_REG_X86_BL:  (*((triton::uint8*)(this->ebx)))   = value.convert_to<triton::uint8>(); break;

          case triton::arch::ID_REG_X86_ECX: (*((triton::uint32*)(this->ecx)))  = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_CX:  (*((triton::uint16*)(this->ecx)))  = value.convert_to<triton::uint16>(); break;
          case triton::arch::ID_REG_X86_CH:  (*((triton::uint8*)(this->ecx+1))) = value.convert_to<triton::uint8>(); break;
          case triton::arch::ID_REG_X86_CL:  (*((triton::uint8*)(this->ecx)))   = value.convert_to<triton::uint8>(); break;

          case triton::arch::ID_REG_X86_EDX: (*((triton::uint32*)(this->edx)))  = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_DX:  (*((triton::uint16*)(this->edx)))  = value.convert_to<triton::uint16>(); break;
          case triton::arch::ID_REG_X86_DH:  (*((triton::uint8*)(this->edx+1))) = value.convert_to<triton::uint8>(); break;
          case triton::arch::ID_REG_X86_DL:  (*((triton::uint8*)(this->edx)))   = value.convert_to<triton::uint8>(); break;

          case triton::arch::ID_REG_X86_EDI: (*((triton::uint32*)(this->edi)))  = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_DI:  (*((triton::uint16*)(this->edi)))  = value.convert_to<triton::uint16>(); break;
          case triton::arch::ID_REG_X86_DIL: (*((triton::uint8*)(this->edi)))   = value.convert_to<triton::uint8>(); break;

          case triton::arch::ID_REG_X86_ESI: (*((triton::uint32*)(this->esi)))  = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_SI:  (*((triton::uint16*)(this->esi)))  = value.convert_to<triton::uint16>(); break;
          case triton::arch::ID_REG_X86_SIL: (*((triton::uint8*)(this->esi)))   = value.convert_to<triton::uint8>(); break;

          case triton::arch::ID_REG_X86_ESP: (*((triton::uint32*)(this->esp)))  = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_SP:  (*((triton::uint16*)(this->esp)))  = value.convert_to<triton::uint16>(); break;
          case triton::arch::ID_REG_X86_SPL: (*((triton::uint8*)(this->esp)))   = value.convert_to<triton::uint8>(); break;

          case triton::arch::ID_REG_X86_EBP: (*((triton::uint32*)(this->ebp)))  = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_BP:  (*((triton::uint16*)(this->ebp)))  = value.convert_to<triton::uint16>(); break;
          case triton::arch::ID_REG_X86_BPL: (*((triton::uint8*)(this->ebp)))   = value.convert_to<triton::uint8>(); break;

          case triton::arch::ID_REG_X86_EIP: (*((triton::uint32*)(this->eip)))  = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_IP:  (*((triton::uint16*)(this->eip)))  = value.convert_to<triton::uint16>(); break;

          case triton::arch::ID_REG_X86_EFLAGS: (*((triton::uint32*)(this->eflags))) = value.convert_to<triton::uint32>(); break;

          case triton::arch::ID_REG_X86_CF: {
            triton::uint32 b = (*((triton::uint32*)(this->eflags)));
            (*((triton::uint32*)(this->eflags))) = !value.is_zero() ? b | (1 << 0) : b & ~(1 << 0);
            break;
          }
          case triton::arch::ID_REG_X86_PF: {
            triton::uint32 b = (*((triton::uint32*)(this->eflags)));
            (*((triton::uint32*)(this->eflags))) = !value.is_zero() ? b | (1 << 2) : b & ~(1 << 2);
            break;
          }
          case triton::arch::ID_REG_X86_AF: {
            triton::uint32 b = (*((triton::uint32*)(this->eflags)));
            (*((triton::uint32*)(this->eflags))) = !value.is_zero() ? b | (1 << 4) : b & ~(1 << 4);
            break;
          }
          case triton::arch::ID_REG_X86_ZF: {
            triton::uint32 b = (*((triton::uint32*)(this->eflags)));
            (*((triton::uint32*)(this->eflags))) = !value.is_zero() ? b | (1 << 6) : b & ~(1 << 6);
            break;
          }
          case triton::arch::ID_REG_X86_SF: {
            triton::uint32 b = (*((triton::uint32*)(this->eflags)));
            (*((triton::uint32*)(this->eflags))) = !value.is_zero() ? b | (1 << 7) : b & ~(1 << 7);
            break;
          }
          case triton::arch::ID_REG_X86_TF: {
            triton::uint32 b = (*((triton::uint32*)(this->eflags)));
            (*((triton::uint32*)(this->eflags))) = !value.is_zero() ? b | (1 << 8) : b & ~(1 << 8);
            break;
          }
          case triton::arch::ID_REG_X86_IF: {
            triton::uint32 b = (*((triton::uint32*)(this->eflags)));
            (*((triton::uint32*)(this->eflags))) = !value.is_zero() ? b | (1 << 9) : b & ~(1 << 9);
            break;
          }
          case triton::arch::ID_REG_X86_DF: {
            triton::uint32 b = (*((triton::uint32*)(this->eflags)));
            (*((triton::uint32*)(this->eflags))) = !value.is_zero() ? b | (1 << 10) : b & ~(1 << 10);
            break;
          }
          case triton::arch::ID_REG_X86_OF: {
            triton::uint32 b = (*((triton::uint32*)(this->eflags)));
            (*((triton::uint32*)(this->eflags))) = !value.is_zero() ? b | (1 << 11) : b & ~(1 << 11);
            break;
          }
          case triton::arch::ID_REG_X86_NT: {
            triton::uint32 b = (*((triton::uint32*)(this->eflags)));
            (*((triton::uint32*)(this->eflags))) = !value.is_zero() ? b | (1 << 14) : b & ~(1 << 14);
            break;
          }
          case triton::arch::ID_REG_X86_RF: {
            triton::uint32 b = (*((triton::uint32*)(this->eflags)));
            (*((triton::uint32*)(this->eflags))) = !value.is_zero() ? b | (1 << 16) : b & ~(1 << 16);
            break;
          }
          case triton::arch::ID_REG_X86_VM: {
            triton::uint32 b = (*((triton::uint32*)(this->eflags)));
            (*((triton::uint32*)(this->eflags))) = !value.is_zero() ? b | (1 << 17) : b & ~(1 << 17);
            break;
          }
          case triton::arch::ID_REG_X86_AC: {
            triton::uint32 b = (*((triton::uint32*)(this->eflags)));
            (*((triton::uint32*)(this->eflags))) = !value.is_zero() ? b | (1 << 18) : b & ~(1 << 18);
            break;
          }
          case triton::arch::ID_REG_X86_VIF: {
            triton::uint32 b = (*((triton::uint32*)(this->eflags)));
            (*((triton::uint32*)(this->eflags))) = !value.is_zero() ? b | (1 << 19) : b & ~(1 << 19);
            break;
          }
          case triton::arch::ID_REG_X86_VIP: {
            triton::uint32 b = (*((triton::uint32*)(this->eflags)));
            (*((triton::uint32*)(this->eflags))) = !value.is_zero() ? b | (1 << 20) : b & ~(1 << 20);
            break;
          }
          case triton::arch::ID_REG_X86_ID: {
            triton::uint32 b = (*((triton::uint32*)(this->eflags)));
            (*((triton::uint32*)(this->eflags))) = !value.is_zero() ? b | (1 << 21) : b & ~(1 << 21);
            break;
          }

          case triton::arch::ID_REG_X86_MM0:  (*((triton::uint64*)(this->mm0))) = value.convert_to<triton::uint64>(); break;
          case triton::arch::ID_REG_X86_MM1:  (*((triton::uint64*)(this->mm1))) = value.convert_to<triton::uint64>(); break;
          case triton::arch::ID_REG_X86_MM2:  (*((triton::uint64*)(this->mm2))) = value.convert_to<triton::uint64>(); break;
          case triton::arch::ID_REG_X86_MM3:  (*((triton::uint64*)(this->mm3))) = value.convert_to<triton::uint64>(); break;
          case triton::arch::ID_REG_X86_MM4:  (*((triton::uint64*)(this->mm4))) = value.convert_to<triton::uint64>(); break;
          case triton::arch::ID_REG_X86_MM5:  (*((triton::uint64*)(this->mm5))) = value.convert_to<triton::uint64>(); break;
          case triton::arch::ID_REG_X86_MM6:  (*((triton::uint64*)(this->mm6))) = value.convert_to<triton::uint64>(); break;
          case triton::arch::ID_REG_X86_MM7:  (*((triton::uint64*)(this->mm7))) = value.convert_to<triton::uint64>(); break;

          case triton::arch::ID_REG_X86_XMM0: triton::utils::fromUintToBuffer(value.convert_to<triton::uint128>(), this->ymm0); break;
          case triton::arch::ID_REG_X86_XMM1: triton::utils::fromUintToBuffer(value.convert_to<triton::uint128>(), this->ymm1); break;
          case triton::arch::ID_REG_X86_XMM2: triton::utils::fromUintToBuffer(value.convert_to<triton::uint128>(), this->ymm2); break;
          case triton::arch::ID_REG_X86_XMM3: triton::utils::fromUintToBuffer(value.convert_to<triton::uint128>(), this->ymm3); break;
          case triton::arch::ID_REG_X86_XMM4: triton::utils::fromUintToBuffer(value.convert_to<triton::uint128>(), this->ymm4); break;
          case triton::arch::ID_REG_X86_XMM5: triton::utils::fromUintToBuffer(value.convert_to<triton::uint128>(), this->ymm5); break;
          case triton::arch::ID_REG_X86_XMM6: triton::utils::fromUintToBuffer(value.convert_to<triton::uint128>(), this->ymm6); break;
          case triton::arch::ID_REG_X86_XMM7: triton::utils::fromUintToBuffer(value.convert_to<triton::uint128>(), this->ymm7); break;

          case triton::arch::ID_REG_X86_YMM0: triton::utils::fromUintToBuffer(value.convert_to<triton::uint256>(), this->ymm0); break;
          case triton::arch::ID_REG_X86_YMM1: triton::utils::fromUintToBuffer(value.convert_to<triton::uint256>(), this->ymm1); break;
          case triton::arch::ID_REG_X86_YMM2: triton::utils::fromUintToBuffer(value.convert_to<triton::uint256>(), this->ymm2); break;
          case triton::arch::ID_REG_X86_YMM3: triton::utils::fromUintToBuffer(value.convert_to<triton::uint256>(), this->ymm3); break;
          case triton::arch::ID_REG_X86_YMM4: triton::utils::fromUintToBuffer(value.convert_to<triton::uint256>(), this->ymm4); break;
          case triton::arch::ID_REG_X86_YMM5: triton::utils::fromUintToBuffer(value.convert_to<triton::uint256>(), this->ymm5); break;
          case triton::arch::ID_REG_X86_YMM6: triton::utils::fromUintToBuffer(value.convert_to<triton::uint256>(), this->ymm6); break;
          case triton::arch::ID_REG_X86_YMM7: triton::utils::fromUintToBuffer(value.convert_to<triton::uint256>(), this->ymm7); break;

          case triton::arch::ID_REG_X86_MXCSR: (*((triton::uint32*)(this->mxcsr))) = value.convert_to<triton::uint32>(); break;

          case triton::arch::ID_REG_X86_IE: {
            triton::uint32 b = (*((triton::uint32*)(this->mxcsr)));
            (*((triton::uint32*)(this->mxcsr))) = !value.is_zero() ? b | (1 << 0) : b & ~(1 << 0);
            break;
          }
          case triton::arch::ID_REG_X86_DE: {
            triton::uint32 b = (*((triton::uint32*)(this->mxcsr)));
            (*((triton::uint32*)(this->mxcsr))) = !value.is_zero() ? b | (1 << 1) : b & ~(1 << 1);
            break;
          }
          case triton::arch::ID_REG_X86_ZE: {
            triton::uint32 b = (*((triton::uint32*)(this->mxcsr)));
            (*((triton::uint32*)(this->mxcsr))) = !value.is_zero() ? b | (1 << 2) : b & ~(1 << 2);
            break;
          }
          case triton::arch::ID_REG_X86_OE: {
            triton::uint32 b = (*((triton::uint32*)(this->mxcsr)));
            (*((triton::uint32*)(this->mxcsr))) = !value.is_zero() ? b | (1 << 3) : b & ~(1 << 3);
            break;
          }
          case triton::arch::ID_REG_X86_UE: {
            triton::uint32 b = (*((triton::uint32*)(this->mxcsr)));
            (*((triton::uint32*)(this->mxcsr))) = !value.is_zero() ? b | (1 << 4) : b & ~(1 << 4);
            break;
          }
          case triton::arch::ID_REG_X86_PE: {
            triton::uint32 b = (*((triton::uint32*)(this->mxcsr)));
            (*((triton::uint32*)(this->mxcsr))) = !value.is_zero() ? b | (1 << 5) : b & ~(1 << 5);
            break;
          }
          case triton::arch::ID_REG_X86_DAZ: {
            triton::uint32 b = (*((triton::uint32*)(this->mxcsr)));
            (*((triton::uint32*)(this->mxcsr))) = !value.is_zero() ? b | (1 << 6) : b & ~(1 << 6);
            break;
          }
          case triton::arch::ID_REG_X86_IM: {
            triton::uint32 b = (*((triton::uint32*)(this->mxcsr)));
            (*((triton::uint32*)(this->mxcsr))) = !value.is_zero() ? b | (1 << 7) : b & ~(1 << 7);
            break;
          }
          case triton::arch::ID_REG_X86_DM: {
            triton::uint32 b = (*((triton::uint32*)(this->mxcsr)));
            (*((triton::uint32*)(this->mxcsr))) = !value.is_zero() ? b | (1 << 8) : b & ~(1 << 8);
            break;
          }
          case triton::arch::ID_REG_X86_ZM: {
            triton::uint32 b = (*((triton::uint32*)(this->mxcsr)));
            (*((triton::uint32*)(this->mxcsr))) = !value.is_zero() ? b | (1 << 9) : b & ~(1 << 9);
            break;
          }
          case triton::arch::ID_REG_X86_OM: {
            triton::uint32 b = (*((triton::uint32*)(this->mxcsr)));
            (*((triton::uint32*)(this->mxcsr))) = !value.is_zero() ? b | (1 << 10) : b & ~(1 << 10);
            break;
          }
          case triton::arch::ID_REG_X86_UM: {
            triton::uint32 b = (*((triton::uint32*)(this->mxcsr)));
            (*((triton::uint32*)(this->mxcsr))) = !value.is_zero() ? b | (1 << 11) : b & ~(1 << 11);
            break;
          }
          case triton::arch::ID_REG_X86_PM: {
            triton::uint32 b = (*((triton::uint32*)(this->mxcsr)));
            (*((triton::uint32*)(this->mxcsr))) = !value.is_zero() ? b | (1 << 12) : b & ~(1 << 12);
            break;
          }
          case triton::arch::ID_REG_X86_RL: {
            triton::uint32 b = (*((triton::uint32*)(this->mxcsr)));
            (*((triton::uint32*)(this->mxcsr))) = !value.is_zero() ? b | (1 << 13) : b & ~(1 << 13);
            break;
          }
          case triton::arch::ID_REG_X86_RH: {
            triton::uint32 b = (*((triton::uint32*)(this->mxcsr)));
            (*((triton::uint32*)(this->mxcsr))) = !value.is_zero() ? b | (1 << 14) : b & ~(1 << 14);
            break;
          }
          case triton::arch::ID_REG_X86_FZ: {
            triton::uint32 b = (*((triton::uint32*)(this->mxcsr)));
            (*((triton::uint32*)(this->mxcsr))) = !value.is_zero() ? b | (1 << 15) : b & ~(1 << 15);
            break;
          }

          case triton::arch::ID_REG_X86_CR0:  (*((triton::uint32*)(this->cr0)))  = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_CR1:  (*((triton::uint32*)(this->cr1)))  = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_CR2:  (*((triton::uint32*)(this->cr2)))  = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_CR3:  (*((triton::uint32*)(this->cr3)))  = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_CR4:  (*((triton::uint32*)(this->cr4)))  = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_CR5:  (*((triton::uint32*)(this->cr5)))  = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_CR6:  (*((triton::uint32*)(this->cr6)))  = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_CR7:  (*((triton::uint32*)(this->cr7)))  = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_CR8:  (*((triton::uint32*)(this->cr8)))  = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_CR9:  (*((triton::uint32*)(this->cr9)))  = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_CR10: (*((triton::uint32*)(this->cr10))) = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_CR11: (*((triton::uint32*)(this->cr11))) = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_CR12: (*((triton::uint32*)(this->cr12))) = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_CR13: (*((triton::uint32*)(this->cr13))) = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_CR14: (*((triton::uint32*)(this->cr14))) = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_CR15: (*((triton::uint32*)(this->cr15))) = value.convert_to<triton::uint32>(); break;

          case triton::arch::ID_REG_X86_DR0:  (*((triton::uint32*)(this->dr0))) = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_DR1:  (*((triton::uint32*)(this->dr1))) = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_DR2:  (*((triton::uint32*)(this->dr2))) = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_DR3:  (*((triton::uint32*)(this->dr3))) = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_DR6:  (*((triton::uint32*)(this->dr6))) = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_DR7:  (*((triton::uint32*)(this->dr7))) = value.convert_to<triton::uint32>(); break;

          case triton::arch::ID_REG_X86_CS:  (*((triton::uint32*)(this->cs))) = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_DS:  (*((triton::uint32*)(this->ds))) = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_ES:  (*((triton::uint32*)(this->es))) = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_FS:  (*((triton::uint32*)(this->fs))) = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_GS:  (*((triton::uint32*)(this->gs))) = value.convert_to<triton::uint32>(); break;
          case triton::arch::ID_REG_X86_SS:  (*((triton::uint32*)(this->ss))) = value.convert_to<triton::uint32>(); break;

          default:
            throw triton::exceptions::Cpu("x86Cpu:setConcreteRegisterValue() - Invalid register.");
        }
      }


      bool x86Cpu::isThumb(void) const {
        /* There is no thumb mode in x86 */
        return false;
      }


      void x86Cpu::setThumb(bool state) {
        /* There is no thumb mode in x86 */
      }


      bool x86Cpu::isMemoryExclusiveAccess(void) const {
        /* There is no exclusive memory access support in x86 */
        return false;
      }


      void x86Cpu::setMemoryExclusiveAccess(bool state) {
        /* There is no exclusive memory access support in x86 */
      }


      bool x86Cpu::isConcreteMemoryValueDefined(const triton::arch::MemoryAccess& mem) const {
        return this->isConcreteMemoryValueDefined(mem.getAddress(), mem.getSize());
      }


      bool x86Cpu::isConcreteMemoryValueDefined(triton::uint64 baseAddr, triton::usize size) const {
        for (triton::usize index = 0; index < size; index++) {
          if (this->memory.find(baseAddr + index) == this->memory.end())
            return false;
        }
        return true;
      }


      void x86Cpu::clearConcreteMemoryValue(const triton::arch::MemoryAccess& mem) {
        this->clearConcreteMemoryValue(mem.getAddress(), mem.getSize());
      }


      void x86Cpu::clearConcreteMemoryValue(triton::uint64 baseAddr, triton::usize size) {
        for (triton::usize index = 0; index < size; index++) {
          if (this->memory.find(baseAddr + index) != this->memory.end()) {
            this->memory.erase(baseAddr + index);
          }
        }
      }

    }; /* x86 namespace */
  }; /* arch namespace */
}; /* triton namespace */
