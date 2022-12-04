#pragma once

namespace fiction::glulx {

class GlulxImpl;

auto ExecuteNop(GlulxImpl&) -> void;

auto ExecuteAdd(GlulxImpl&) -> void;
auto ExecuteSub(GlulxImpl&) -> void;
auto ExecuteMul(GlulxImpl&) -> void;
auto ExecuteDiv(GlulxImpl&) -> void;
auto ExecuteMod(GlulxImpl&) -> void;
auto ExecuteNeg(GlulxImpl&) -> void;
auto ExecuteBitand(GlulxImpl&) -> void;
auto ExecuteBitor(GlulxImpl&) -> void;
auto ExecuteBitxor(GlulxImpl&) -> void;
auto ExecuteBitnot(GlulxImpl&) -> void;
auto ExecuteShiftl(GlulxImpl&) -> void;
auto ExecuteSshiftr(GlulxImpl&) -> void;
auto ExecuteUshiftr(GlulxImpl&) -> void;

auto ExecuteJump(GlulxImpl&) -> void;
auto ExecuteJz(GlulxImpl&) -> void;
auto ExecuteJnz(GlulxImpl&) -> void;
auto ExecuteJeq(GlulxImpl&) -> void;
auto ExecuteJne(GlulxImpl&) -> void;
auto ExecuteJlt(GlulxImpl&) -> void;
auto ExecuteJge(GlulxImpl&) -> void;
auto ExecuteJgt(GlulxImpl&) -> void;
auto ExecuteJle(GlulxImpl&) -> void;
auto ExecuteJltu(GlulxImpl&) -> void;
auto ExecuteJgeu(GlulxImpl&) -> void;
auto ExecuteJgtu(GlulxImpl&) -> void;
auto ExecuteJleu(GlulxImpl&) -> void;

auto ExecuteCall(GlulxImpl&) -> void;
auto ExecuteReturn(GlulxImpl&) -> void;
auto ExecuteCatch(GlulxImpl&) -> void;
auto ExecuteThrow(GlulxImpl&) -> void;
auto ExecuteTailcall(GlulxImpl&) -> void;

auto ExecuteCopy(GlulxImpl&) -> void;
auto ExecuteCopys(GlulxImpl&) -> void;
auto ExecuteCopyb(GlulxImpl&) -> void;
auto ExecuteSexs(GlulxImpl&) -> void;
auto ExecuteSexb(GlulxImpl&) -> void;

auto ExecuteAload(GlulxImpl&) -> void;
auto ExecuteAloads(GlulxImpl&) -> void;
auto ExecuteAloadb(GlulxImpl&) -> void;
auto ExecuteAloadbit(GlulxImpl&) -> void;
auto ExecuteAstore(GlulxImpl&) -> void;
auto ExecuteAstores(GlulxImpl&) -> void;
auto ExecuteAstoreb(GlulxImpl&) -> void;
auto ExecuteAstorebit(GlulxImpl&) -> void;

auto ExecuteStkcount(GlulxImpl&) -> void;
auto ExecuteStkpeek(GlulxImpl&) -> void;
auto ExecuteStkswap(GlulxImpl&) -> void;
auto ExecuteStkroll(GlulxImpl&) -> void;
auto ExecuteStkcopy(GlulxImpl&) -> void;

auto ExecuteStreamchar(GlulxImpl&) -> void;
auto ExecuteStreamnum(GlulxImpl&) -> void;
auto ExecuteStreamstr(GlulxImpl&) -> void;
auto ExecuteStreamunichar(GlulxImpl&) -> void;

auto ExecuteGestalt(GlulxImpl&) -> void;
auto ExecuteDebugtrap(GlulxImpl&) -> void;
auto ExecuteGetmemsize(GlulxImpl&) -> void;
auto ExecuteSetmemsize(GlulxImpl&) -> void;
auto ExecuteJumpabs(GlulxImpl&) -> void;

auto ExecuteRandom(GlulxImpl&) -> void;
auto ExecuteSetrandom(GlulxImpl&) -> void;

auto ExecuteQuit(GlulxImpl&) -> void;
auto ExecuteVerify(GlulxImpl&) -> void;
auto ExecuteRestart(GlulxImpl&) -> void;
auto ExecuteSave(GlulxImpl&) -> void;
auto ExecuteRestore(GlulxImpl&) -> void;
auto ExecuteSaveundo(GlulxImpl&) -> void;
auto ExecuteRestoreundo(GlulxImpl&) -> void;
auto ExecuteProtect(GlulxImpl&) -> void;
auto ExecuteHasundo(GlulxImpl&) -> void;
auto ExecuteDiscardundo(GlulxImpl&) -> void;

auto ExecuteGlk(GlulxImpl&) -> void;

auto ExecuteGetstringtbl(GlulxImpl&) -> void;
auto ExecuteSetstringtbl(GlulxImpl&) -> void;
auto ExecuteGetiosys(GlulxImpl&) -> void;
auto ExecuteSetiosys(GlulxImpl&) -> void;

auto ExecuteLinearsearch(GlulxImpl&) -> void;
auto ExecuteBinarysearch(GlulxImpl&) -> void;
auto ExecuteLinkedsearch(GlulxImpl&) -> void;

auto ExecuteCallf(GlulxImpl&) -> void;
auto ExecuteCallfi(GlulxImpl&) -> void;
auto ExecuteCallfii(GlulxImpl&) -> void;
auto ExecuteCallfiii(GlulxImpl&) -> void;

auto ExecuteMzero(GlulxImpl&) -> void;
auto ExecuteMcopy(GlulxImpl&) -> void;
auto ExecuteMalloc(GlulxImpl&) -> void;
auto ExecuteMfree(GlulxImpl&) -> void;

auto ExecuteAccelfunc(GlulxImpl&) -> void;
auto ExecuteAccelparam(GlulxImpl&) -> void;

auto ExecuteNumtof(GlulxImpl&) -> void;
auto ExecuteFtonumz(GlulxImpl&) -> void;
auto ExecuteFtonumn(GlulxImpl&) -> void;
auto ExecuteCeil(GlulxImpl&) -> void;
auto ExecuteFloor(GlulxImpl&) -> void;

auto ExecuteFadd(GlulxImpl&) -> void;
auto ExecuteFsub(GlulxImpl&) -> void;
auto ExecuteFmul(GlulxImpl&) -> void;
auto ExecuteFdiv(GlulxImpl&) -> void;
auto ExecuteFmod(GlulxImpl&) -> void;
auto ExecuteSqrt(GlulxImpl&) -> void;
auto ExecuteExp(GlulxImpl&) -> void;
auto ExecuteLog(GlulxImpl&) -> void;
auto ExecutePow(GlulxImpl&) -> void;

auto ExecuteSin(GlulxImpl&) -> void;
auto ExecuteCos(GlulxImpl&) -> void;
auto ExecuteTan(GlulxImpl&) -> void;
auto ExecuteAsin(GlulxImpl&) -> void;
auto ExecuteAcos(GlulxImpl&) -> void;
auto ExecuteAtan(GlulxImpl&) -> void;
auto ExecuteAtan2(GlulxImpl&) -> void;

auto ExecuteJfeq(GlulxImpl&) -> void;
auto ExecuteJfne(GlulxImpl&) -> void;
auto ExecuteJflt(GlulxImpl&) -> void;
auto ExecuteJfle(GlulxImpl&) -> void;
auto ExecuteJfgt(GlulxImpl&) -> void;
auto ExecuteJfge(GlulxImpl&) -> void;
auto ExecuteJisnan(GlulxImpl&) -> void;
auto ExecuteJisinf(GlulxImpl&) -> void;

auto ExecuteNumtod(GlulxImpl&) -> void;
auto ExecuteDtonumz(GlulxImpl&) -> void;
auto ExecuteDtonumn(GlulxImpl&) -> void;
auto ExecuteFtod(GlulxImpl&) -> void;
auto ExecuteDtof(GlulxImpl&) -> void;
auto ExecuteDceil(GlulxImpl&) -> void;
auto ExecuteDfloor(GlulxImpl&) -> void;

auto ExecuteDadd(GlulxImpl&) -> void;
auto ExecuteDsub(GlulxImpl&) -> void;
auto ExecuteDmul(GlulxImpl&) -> void;
auto ExecuteDdiv(GlulxImpl&) -> void;
auto ExecuteDmodr(GlulxImpl&) -> void;
auto ExecuteDmodq(GlulxImpl&) -> void;
auto ExecuteDsqrt(GlulxImpl&) -> void;
auto ExecuteDexp(GlulxImpl&) -> void;
auto ExecuteDlog(GlulxImpl&) -> void;
auto ExecuteDpow(GlulxImpl&) -> void;

auto ExecuteDsin(GlulxImpl&) -> void;
auto ExecuteDcos(GlulxImpl&) -> void;
auto ExecuteDtan(GlulxImpl&) -> void;
auto ExecuteDasin(GlulxImpl&) -> void;
auto ExecuteDacos(GlulxImpl&) -> void;
auto ExecuteDatan(GlulxImpl&) -> void;
auto ExecuteDatan2(GlulxImpl&) -> void;

auto ExecuteJdeq(GlulxImpl&) -> void;
auto ExecuteJdne(GlulxImpl&) -> void;
auto ExecuteJdlt(GlulxImpl&) -> void;
auto ExecuteJdle(GlulxImpl&) -> void;
auto ExecuteJdgt(GlulxImpl&) -> void;
auto ExecuteJdge(GlulxImpl&) -> void;
auto ExecuteJdisnan(GlulxImpl&) -> void;
auto ExecuteJdisinf(GlulxImpl&) -> void;

}