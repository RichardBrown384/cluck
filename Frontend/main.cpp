#include "Glulx.h"
#include "GlkServer.h"

#include "GlkGlulxClient.h"
#include "GlkInputOutputSystem.h"

#include "FileSystem.h"

void GlkMain(const std::vector<std::string>& arguments) {

    using namespace fiction::glk;
    using namespace fiction::glulx;

    if (arguments.size() < 2u) {
        return;
    }

    Glulx glulx(fiction::filesystem::LoadFile(arguments[1u]));
    if (!glulx.IsFileValid()) {
        return;
    }

    GlkGlulxClient client(glulx);
    GlkServer server(client);

    GlkInputOutputSystem io(server);
    glulx.SetInputOutputSystem(IO_SYSTEM_GLK, io);

    glulx.SetServerFunction(
        [&](uint32_t function, const std::vector<uint32_t>& arguments) {
            return server.Call(function, arguments);
        }
    );

    glulx.Restart();

    while (glulx.IsRunning()) {
        server.Tick();
        glulx.Execute();
    }
}
