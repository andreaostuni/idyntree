// SPDX-FileCopyrightText: Fondazione Istituto Italiano di Tecnologia (IIT)
// SPDX-License-Identifier: BSD-3-Clause

#include <iDynTree/Model.h>
#include <iDynTree/ModelLoader.h>

#include <iDynTree/Visualizer.h>

#include "cmdline.h"

#include <iostream>
#include <cstdlib>

/**
 * Add the option supported by the idyntree-model-info utility
 */
void addOptions(cmdline::parser &cmd)
{
    cmd.add<std::string>("model", 'm',
                         "Model to load.",
                         true);

    cmd.add<std::string>("color-palette", 'c',
                         "Color palette.",
                         false);
}

int main(int argc, char** argv)
{
    cmdline::parser cmd;
    addOptions(cmd);
    cmd.parse_check(argc, argv);

    // Read model
    const std::string& modelPath = cmd.get<std::string>("model");
    iDynTree::ModelLoader mdlLoader;
    bool ok = mdlLoader.loadModelFromFile(modelPath);

    if( !ok )
    {
        std::cerr << "Impossible to read model at file " << modelPath << std::endl;
        return EXIT_FAILURE;
    }

    // Visualize the model
    iDynTree::Visualizer visualizer;
    ok = visualizer.addModel(mdlLoader.model(),"model");
    const std::string& palette = cmd.get<std::string>("color-palette");
    if(!palette.empty())
    {
        ok = visualizer.setColorPalette(palette);
        if( !ok )
        {
            std::cerr << "Impossible to set the color palette." << std::endl;
            return EXIT_FAILURE;
        }
    }

    visualizer.camera().animator()->enableMouseControl();

    if( !ok )
    {
        std::cerr << "Impossible to read visualizer." << std::endl;
        return EXIT_FAILURE;
    }

    // Visualization loop
    while( visualizer.run() )
    {
        visualizer.draw();
    }

    return EXIT_SUCCESS;
}
