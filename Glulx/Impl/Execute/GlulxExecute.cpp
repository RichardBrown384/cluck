#include "Impl/GlulxError.h"

#include "Impl/Execute/Ops/GlulxOps.h"

#include "Impl/Execute/Instruction/GlulxInstruction.h"

namespace {
constexpr auto OPCODE_NOP = 0x00u;

constexpr auto OPCODE_ADD = 0x10u;
constexpr auto OPCODE_SUB = 0x11u;
constexpr auto OPCODE_MUL = 0x12u;
constexpr auto OPCODE_DIV = 0x13u;
constexpr auto OPCODE_MOD = 0x14u;
constexpr auto OPCODE_NEG = 0x15u;
constexpr auto OPCODE_BITAND = 0x18u;
constexpr auto OPCODE_BITOR = 0x19u;
constexpr auto OPCODE_BITXOR = 0x1Au;
constexpr auto OPCODE_BITNOT = 0x1Bu;
constexpr auto OPCODE_SHIFTL = 0x1Cu;
constexpr auto OPCODE_SSHIFTR = 0x1Du;
constexpr auto OPCODE_USHIFTR = 0x1Eu;

constexpr auto OPCODE_JUMP = 0x20u;
constexpr auto OPCODE_JZ = 0x22u;
constexpr auto OPCODE_JNZ = 0x23u;
constexpr auto OPCODE_JEQ = 0x24u;
constexpr auto OPCODE_JNE = 0x25u;
constexpr auto OPCODE_JLT = 0x26u;
constexpr auto OPCODE_JGE = 0x27u;
constexpr auto OPCODE_JGT = 0x28u;
constexpr auto OPCODE_JLE = 0x29u;
constexpr auto OPCODE_JLTU = 0x2Au;
constexpr auto OPCODE_JGEU = 0x2Bu;
constexpr auto OPCODE_JGTU = 0x2Cu;
constexpr auto OPCODE_JLEU = 0x2Du;

constexpr auto OPCODE_CALL = 0x30u;
constexpr auto OPCODE_RETURN = 0x31u;
constexpr auto OPCODE_CATCH = 0x32u;
constexpr auto OPCODE_THROW = 0x33u;
constexpr auto OPCODE_TAILCALL = 0x34u;

constexpr auto OPCODE_COPY = 0x40u;
constexpr auto OPCODE_COPYS = 0x41u;
constexpr auto OPCODE_COPYB = 0x42u;
constexpr auto OPCODE_SEXS = 0x44u;
constexpr auto OPCODE_SEXB = 0x45u;

constexpr auto OPCODE_ALOAD = 0x48u;
constexpr auto OPCODE_ALOADS = 0x49u;
constexpr auto OPCODE_ALOADB = 0x4Au;
constexpr auto OPCODE_ALOADBIT = 0x4Bu;
constexpr auto OPCODE_ASTORE = 0x4Cu;
constexpr auto OPCODE_ASTORES = 0x4Du;
constexpr auto OPCODE_ASTOREB = 0x4Eu;
constexpr auto OPCODE_ASTOREBIT = 0x4Fu;

constexpr auto OPCODE_STKCOUNT = 0x50u;
constexpr auto OPCODE_STKPEEK = 0x51u;
constexpr auto OPCODE_STKSWAP = 0x52u;
constexpr auto OPCODE_STKROLL = 0x53u;
constexpr auto OPCODE_STKCOPY = 0x54u;

constexpr auto OPCODE_STREAMCHAR = 0x70u;
constexpr auto OPCODE_STREAMNUM = 0x71u;
constexpr auto OPCODE_STREAMSTR = 0x72u;
constexpr auto OPCODE_STREAMUNICHAR = 0x73u;

constexpr auto OPCODE_GESTALT = 0x100u;
constexpr auto OPCODE_DEBUGTRAP = 0x101u;
constexpr auto OPCODE_GETMEMSIZE = 0x102u;
constexpr auto OPCODE_SETMEMSIZE = 0x103u;
constexpr auto OPCODE_JUMPABS = 0x104u;

constexpr auto OPCODE_RANDOM = 0x110u;
constexpr auto OPCODE_SETRANDOM = 0x111u;

constexpr auto OPCODE_QUIT = 0x120u;
constexpr auto OPCODE_VERIFY = 0x121u;
constexpr auto OPCODE_RESTART = 0x122u;
constexpr auto OPCODE_SAVE = 0x123u;
constexpr auto OPCODE_RESTORE = 0x124u;
constexpr auto OPCODE_SAVEUNDO = 0x125u;
constexpr auto OPCODE_RESTOREUNDO = 0x126u;
constexpr auto OPCODE_PROTECT = 0x127u;
constexpr auto OPCODE_HASUNDO = 0x128u;
constexpr auto OPCODE_DISCARDUNDO = 0x129u;

constexpr auto OPCODE_GLK = 0x130u;

constexpr auto OPCODE_GETSTRINGTBL = 0x140u;
constexpr auto OPCODE_SETSTRINGTBL = 0x141u;
constexpr auto OPCODE_GETIOSYS = 0x148u;
constexpr auto OPCODE_SETIOSYS = 0x149u;

constexpr auto OPCODE_LINEARSEARCH = 0x150u;
constexpr auto OPCODE_BINARYSEARCH = 0x151u;
constexpr auto OPCODE_LINKEDSEARCH = 0x152u;

constexpr auto OPCODE_CALLF = 0x160u;
constexpr auto OPCODE_CALLFI = 0x161u;
constexpr auto OPCODE_CALLFII = 0x162u;
constexpr auto OPCODE_CALLFIII = 0x163u;

constexpr auto OPCODE_MZERO = 0x170u;
constexpr auto OPCODE_MCOPY = 0x171u;
constexpr auto OPCODE_MALLOC = 0x178u;
constexpr auto OPCODE_MFREE = 0x179u;

constexpr auto OPCODE_ACCELFUNC = 0x180u;
constexpr auto OPCODE_ACCELPARAM = 0x181u;

constexpr auto OPCODE_NUMTOF = 0x190u;
constexpr auto OPCODE_FTONUMZ = 0x191u;
constexpr auto OPCODE_FTONUMN = 0x192u;
constexpr auto OPCODE_CEIL = 0x198u;
constexpr auto OPCODE_FLOOR = 0x199u;

constexpr auto OPCODE_FADD = 0x1A0u;
constexpr auto OPCODE_FSUB = 0x1A1u;
constexpr auto OPCODE_FMUL = 0x1A2u;
constexpr auto OPCODE_FDIV = 0x1A3u;
constexpr auto OPCODE_FMOD = 0x1A4u;
constexpr auto OPCODE_SQRT = 0x1A8u;
constexpr auto OPCODE_EXP = 0x1A9u;
constexpr auto OPCODE_LOG = 0x1AAu;
constexpr auto OPCODE_POW = 0x1ABu;

constexpr auto OPCODE_SIN = 0x1B0u;
constexpr auto OPCODE_COS = 0x1B1u;
constexpr auto OPCODE_TAN = 0x1B2u;
constexpr auto OPCODE_ASIN = 0x1B3u;
constexpr auto OPCODE_ACOS = 0x1B4u;
constexpr auto OPCODE_ATAN = 0x1B5u;
constexpr auto OPCODE_ATAN2 = 0x1B6u;

constexpr auto OPCODE_JFEQ = 0x1C0u;
constexpr auto OPCODE_JFNE = 0x1C1u;
constexpr auto OPCODE_JFLT = 0x1C2u;
constexpr auto OPCODE_JFLE = 0x1C3u;
constexpr auto OPCODE_JFGT = 0x1C4u;
constexpr auto OPCODE_JFGE = 0x1C5u;
constexpr auto OPCODE_JISNAN = 0x1C8u;
constexpr auto OPCODE_JISINF = 0x1C9u;

constexpr auto OPCODE_NUMTOD = 0x200u;
constexpr auto OPCODE_DTONUMZ = 0x201u;
constexpr auto OPCODE_DTONUMN = 0x202u;
constexpr auto OPCODE_FTOD = 0x203u;
constexpr auto OPCODE_DTOF = 0x204u;
constexpr auto OPCODE_DCEIL = 0x208u;
constexpr auto OPCODE_DFLOOR = 0x209u;

constexpr auto OPCODE_DADD = 0x210u;
constexpr auto OPCODE_DSUB = 0x211u;
constexpr auto OPCODE_DMUL = 0x212u;
constexpr auto OPCODE_DDIV = 0x213u;
constexpr auto OPCODE_DMODR = 0x214u;
constexpr auto OPCODE_DMODQ = 0x215u;
constexpr auto OPCODE_DSQRT = 0x218u;
constexpr auto OPCODE_DEXP = 0x219u;
constexpr auto OPCODE_DLOG = 0x21Au;
constexpr auto OPCODE_DPOW = 0x21Bu;

constexpr auto OPCODE_DSIN = 0x220u;
constexpr auto OPCODE_DCOS = 0x221u;
constexpr auto OPCODE_DTAN = 0x222u;
constexpr auto OPCODE_DASIN = 0x223u;
constexpr auto OPCODE_DACOS = 0x224u;
constexpr auto OPCODE_DATAN = 0x225u;
constexpr auto OPCODE_DATAN2 = 0x226u;

constexpr auto OPCODE_JDEQ = 0x230u;
constexpr auto OPCODE_JDNE = 0x231u;
constexpr auto OPCODE_JDLT = 0x232u;
constexpr auto OPCODE_JDLE = 0x233u;
constexpr auto OPCODE_JDGT = 0x234u;
constexpr auto OPCODE_JDGE = 0x235u;
constexpr auto OPCODE_JDISNAN = 0x238u;
constexpr auto OPCODE_JDISINF = 0x239u;
}

namespace fiction::glulx {

void Execute(GlulxImpl& glulx) {
    const auto opcode = OpcodeRead(glulx);
    switch (opcode) {
        case OPCODE_NOP: return ExecuteNop(glulx);

        case OPCODE_ADD: return ExecuteAdd(glulx);
        case OPCODE_SUB: return ExecuteSub(glulx);
        case OPCODE_MUL: return ExecuteMul(glulx);
        case OPCODE_DIV: return ExecuteDiv(glulx);
        case OPCODE_MOD: return ExecuteMod(glulx);
        case OPCODE_NEG: return ExecuteNeg(glulx);
        case OPCODE_BITAND: return ExecuteBitand(glulx);
        case OPCODE_BITOR: return ExecuteBitor(glulx);
        case OPCODE_BITXOR: return ExecuteBitxor(glulx);
        case OPCODE_BITNOT: return ExecuteBitnot(glulx);
        case OPCODE_SHIFTL: return ExecuteShiftl(glulx);
        case OPCODE_SSHIFTR: return ExecuteSshiftr(glulx);
        case OPCODE_USHIFTR: return ExecuteUshiftr(glulx);

        case OPCODE_JUMP: return ExecuteJump(glulx);
        case OPCODE_JZ: return ExecuteJz(glulx);
        case OPCODE_JNZ: return ExecuteJnz(glulx);
        case OPCODE_JEQ: return ExecuteJeq(glulx);
        case OPCODE_JNE: return ExecuteJne(glulx);
        case OPCODE_JLT: return ExecuteJlt(glulx);
        case OPCODE_JGE: return ExecuteJge(glulx);
        case OPCODE_JGT: return ExecuteJgt(glulx);
        case OPCODE_JLE: return ExecuteJle(glulx);
        case OPCODE_JLTU: return ExecuteJltu(glulx);
        case OPCODE_JGEU: return ExecuteJgeu(glulx);
        case OPCODE_JGTU: return ExecuteJgtu(glulx);
        case OPCODE_JLEU: return ExecuteJleu(glulx);

        case OPCODE_CALL: return ExecuteCall(glulx);
        case OPCODE_RETURN: return ExecuteReturn(glulx);
        case OPCODE_CATCH: return ExecuteCatch(glulx);
        case OPCODE_THROW: return ExecuteThrow(glulx);
        case OPCODE_TAILCALL: return ExecuteTailcall(glulx);

        case OPCODE_COPY: return ExecuteCopy(glulx);
        case OPCODE_COPYS: return ExecuteCopys(glulx);
        case OPCODE_COPYB: return ExecuteCopyb(glulx);
        case OPCODE_SEXS: return ExecuteSexs(glulx);
        case OPCODE_SEXB: return ExecuteSexb(glulx);

        case OPCODE_ALOAD: return ExecuteAload(glulx);
        case OPCODE_ALOADS: return ExecuteAloads(glulx);
        case OPCODE_ALOADB: return ExecuteAloadb(glulx);
        case OPCODE_ALOADBIT: return ExecuteAloadbit(glulx);
        case OPCODE_ASTORE: return ExecuteAstore(glulx);
        case OPCODE_ASTORES: return ExecuteAstores(glulx);
        case OPCODE_ASTOREB: return ExecuteAstoreb(glulx);
        case OPCODE_ASTOREBIT: return ExecuteAstorebit(glulx);

        case OPCODE_STKCOUNT: return ExecuteStkcount(glulx);
        case OPCODE_STKPEEK: return ExecuteStkpeek(glulx);
        case OPCODE_STKSWAP: return ExecuteStkswap(glulx);
        case OPCODE_STKROLL: return ExecuteStkroll(glulx);
        case OPCODE_STKCOPY: return ExecuteStkcopy(glulx);

        case OPCODE_STREAMCHAR: return ExecuteStreamchar(glulx);
        case OPCODE_STREAMNUM: return ExecuteStreamnum(glulx);
        case OPCODE_STREAMSTR: return ExecuteStreamstr(glulx);
        case OPCODE_STREAMUNICHAR: return ExecuteStreamunichar(glulx);

        case OPCODE_GESTALT: return ExecuteGestalt(glulx);
        case OPCODE_DEBUGTRAP: return ExecuteDebugtrap(glulx);
        case OPCODE_GETMEMSIZE: return ExecuteGetmemsize(glulx);
        case OPCODE_SETMEMSIZE: return ExecuteSetmemsize(glulx);
        case OPCODE_JUMPABS: return ExecuteJumpabs(glulx);

        case OPCODE_RANDOM: return ExecuteRandom(glulx);
        case OPCODE_SETRANDOM: return ExecuteSetrandom(glulx);

        case OPCODE_QUIT: return ExecuteQuit(glulx);
        case OPCODE_VERIFY: return ExecuteVerify(glulx);
        case OPCODE_RESTART: return ExecuteRestart(glulx);
        case OPCODE_SAVE: return ExecuteSave(glulx);
        case OPCODE_RESTORE: return ExecuteRestore(glulx);
        case OPCODE_SAVEUNDO: return ExecuteSaveundo(glulx);
        case OPCODE_RESTOREUNDO: return ExecuteRestoreundo(glulx);
        case OPCODE_PROTECT: return ExecuteProtect(glulx);
        case OPCODE_HASUNDO: return ExecuteHasundo(glulx);
        case OPCODE_DISCARDUNDO: return ExecuteDiscardundo(glulx);

        case OPCODE_GLK: return ExecuteGlk(glulx);

        case OPCODE_GETSTRINGTBL: return ExecuteGetstringtbl(glulx);
        case OPCODE_SETSTRINGTBL: return ExecuteSetstringtbl(glulx);
        case OPCODE_GETIOSYS: return ExecuteGetiosys(glulx);
        case OPCODE_SETIOSYS: return ExecuteSetiosys(glulx);

        case OPCODE_LINEARSEARCH: return ExecuteLinearsearch(glulx);
        case OPCODE_BINARYSEARCH: return ExecuteBinarysearch(glulx);
        case OPCODE_LINKEDSEARCH: return ExecuteLinkedsearch(glulx);

        case OPCODE_CALLF: return ExecuteCallf(glulx);
        case OPCODE_CALLFI: return ExecuteCallfi(glulx);
        case OPCODE_CALLFII: return ExecuteCallfii(glulx);
        case OPCODE_CALLFIII: return ExecuteCallfiii(glulx);

        case OPCODE_MZERO: return ExecuteMzero(glulx);
        case OPCODE_MCOPY: return ExecuteMcopy(glulx);
        case OPCODE_MALLOC: return ExecuteMalloc(glulx);
        case OPCODE_MFREE: return ExecuteMfree(glulx);

        case OPCODE_ACCELFUNC: return ExecuteAccelfunc(glulx);
        case OPCODE_ACCELPARAM: return ExecuteAccelparam(glulx);

        case OPCODE_NUMTOF: return ExecuteNumtof(glulx);
        case OPCODE_FTONUMZ: return ExecuteFtonumz(glulx);
        case OPCODE_FTONUMN: return ExecuteFtonumn(glulx);
        case OPCODE_CEIL: return ExecuteCeil(glulx);
        case OPCODE_FLOOR: return ExecuteFloor(glulx);

        case OPCODE_FADD: return ExecuteFadd(glulx);
        case OPCODE_FSUB: return ExecuteFsub(glulx);
        case OPCODE_FMUL: return ExecuteFmul(glulx);
        case OPCODE_FDIV: return ExecuteFdiv(glulx);
        case OPCODE_FMOD: return ExecuteFmod(glulx);
        case OPCODE_SQRT: return ExecuteSqrt(glulx);
        case OPCODE_EXP: return ExecuteExp(glulx);
        case OPCODE_LOG: return ExecuteLog(glulx);
        case OPCODE_POW: return ExecutePow(glulx);

        case OPCODE_SIN: return ExecuteSin(glulx);
        case OPCODE_COS: return ExecuteCos(glulx);
        case OPCODE_TAN: return ExecuteTan(glulx);
        case OPCODE_ASIN: return ExecuteAsin(glulx);
        case OPCODE_ACOS: return ExecuteAcos(glulx);
        case OPCODE_ATAN: return ExecuteAtan(glulx);
        case OPCODE_ATAN2: return ExecuteAtan2(glulx);

        case OPCODE_JFEQ: return ExecuteJfeq(glulx);
        case OPCODE_JFNE: return ExecuteJfne(glulx);
        case OPCODE_JFLT: return ExecuteJflt(glulx);
        case OPCODE_JFLE: return ExecuteJfle(glulx);
        case OPCODE_JFGT: return ExecuteJfgt(glulx);
        case OPCODE_JFGE: return ExecuteJfge(glulx);
        case OPCODE_JISNAN: return ExecuteJisnan(glulx);
        case OPCODE_JISINF: return ExecuteJisinf(glulx);

        case OPCODE_NUMTOD: return ExecuteNumtod(glulx);
        case OPCODE_DTONUMZ: return ExecuteDtonumz(glulx);
        case OPCODE_DTONUMN: return ExecuteDtonumn(glulx);
        case OPCODE_FTOD: return ExecuteFtod(glulx);
        case OPCODE_DTOF: return ExecuteDtof(glulx);
        case OPCODE_DCEIL: return ExecuteDceil(glulx);
        case OPCODE_DFLOOR: return ExecuteDfloor(glulx);

        case OPCODE_DADD: return ExecuteDadd(glulx);
        case OPCODE_DSUB: return ExecuteDsub(glulx);
        case OPCODE_DMUL: return ExecuteDmul(glulx);
        case OPCODE_DDIV: return ExecuteDdiv(glulx);
        case OPCODE_DMODR: return ExecuteDmodr(glulx);
        case OPCODE_DMODQ: return ExecuteDmodq(glulx);
        case OPCODE_DSQRT: return ExecuteDsqrt(glulx);
        case OPCODE_DEXP: return ExecuteDexp(glulx);
        case OPCODE_DLOG: return ExecuteDlog(glulx);
        case OPCODE_DPOW: return ExecuteDpow(glulx);

        case OPCODE_DSIN: return ExecuteDsin(glulx);
        case OPCODE_DCOS: return ExecuteDcos(glulx);
        case OPCODE_DTAN: return ExecuteDtan(glulx);
        case OPCODE_DASIN: return ExecuteDasin(glulx);
        case OPCODE_DACOS: return ExecuteDacos(glulx);
        case OPCODE_DATAN: return ExecuteDatan(glulx);
        case OPCODE_DATAN2: return ExecuteDatan2(glulx);

        case OPCODE_JDEQ: return ExecuteJdeq(glulx);
        case OPCODE_JDNE: return ExecuteJdne(glulx);
        case OPCODE_JDLT: return ExecuteJdlt(glulx);
        case OPCODE_JDLE: return ExecuteJdle(glulx);
        case OPCODE_JDGT: return ExecuteJdgt(glulx);
        case OPCODE_JDGE: return ExecuteJdge(glulx);
        case OPCODE_JDISNAN: return ExecuteJdisnan(glulx);
        case OPCODE_JDISINF: return ExecuteJdisinf(glulx);

        default: Error("Unsupported opcode");
    }

}
}
