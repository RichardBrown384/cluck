#include "ProgramArguments/GlkProgramArguments.h"
#include "Array/GlkArrayRegistry.h"
#include "Object/GlkObjectRegistry.h"

using namespace fiction::glk;

void GlkMain(const std::vector<std::string>&);

extern "C" {
#include "glk.h"
#include "glkstart.h"
#include "gi_dispa.h"

auto ObjectRegister(void* object, glui32 objectClass) -> gidispatch_rock_t {
    auto& registry = GlkObjectRegistry::GetInstance();
    const auto result = registry.Register(object, objectClass);
    return { .num = result };
}

auto ObjectUnregister(void* object, glui32 objectClass, gidispatch_rock_t dispatchRock) -> void {
    auto& registry = GlkObjectRegistry::GetInstance();
    registry.Unregister(object, objectClass, dispatchRock.num);
}

auto ArrayRegister(void* array, glui32 length, char* typeCode) -> gidispatch_rock_t {
    auto& registry = GlkArrayRegistry::GetInstance();
    const auto result = registry.Register(array, length, typeCode);
    return { . num = result };
}

auto ArrayUnregister(void* array, glui32 length, char* typeCode, gidispatch_rock_t dispatchRock) -> void {
    auto& registry = GlkArrayRegistry::GetInstance();
    registry.Unregister(array, length, typeCode, dispatchRock.num);
}

glkunix_argumentlist_t glkunix_arguments[] = {
    { const_cast<char*>(""), glkunix_arg_ValueFollows, nullptr },
    { nullptr, glkunix_arg_End, nullptr }
};

int glkunix_startup_code(glkunix_startup_t* startup) {
    auto& arguments = GlkProgramArguments::GetInstance().GetArguments();
    for (auto i = 0; i < startup->argc; ++i) {
        arguments.emplace_back(startup->argv[i]);
    }
    return TRUE;
}

void glk_main() {
    gidispatch_set_object_registry(ObjectRegister, ObjectUnregister);
    gidispatch_set_retained_registry(ArrayRegister, ArrayUnregister);

    GlkMain(GlkProgramArguments::GetInstance().GetArguments());

    gidispatch_set_retained_registry(nullptr, nullptr);
    gidispatch_set_object_registry(nullptr, nullptr);
}

}