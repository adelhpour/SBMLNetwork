#include "libsbml_ne_render_helpers.h"
#include "libsbml_ne_layout_helpers.h"

#include <cmath>

namespace LIBSBML_NETWORKEDITOR_CPP_NAMESPACE  {

SBasePlugin* getRenderPlugin(SBMLDocument* document) {
    LayoutModelPlugin* layoutModelPlugin = getLayoutModelPlugin(document);
    if (layoutModelPlugin) {
        SBasePlugin* renderBase = getRenderPlugin(layoutModelPlugin);
        if (!renderBase) {
            enableRenderPlugin(document);
            return getRenderPlugin(layoutModelPlugin);
        }
        return renderBase;
    }
    
    return NULL;
}

SBasePlugin* getRenderPlugin(Layout* layout) {
    SBasePlugin* renderBase = layout->getPlugin("render");
    if (!renderBase) {
        enableRenderPlugin(layout);
        return getRenderPlugin(layout);
    }
    return renderBase;
}

SBasePlugin* getRenderPlugin(LayoutModelPlugin* layoutModelPlugin) {
    ListOfLayouts* listOfLayouts = layoutModelPlugin->getListOfLayouts();
    return listOfLayouts->getPlugin("render");
}

RenderListOfLayoutsPlugin* getRenderListOfLayoutsPlugin(SBasePlugin* renderBase) {
    RenderListOfLayoutsPlugin* renderListOfLayoutsPlugin = NULL;
    try {
        renderListOfLayoutsPlugin = dynamic_cast<RenderListOfLayoutsPlugin*>(renderBase); }
    catch(std::bad_cast) {
        std::cerr << "Unable to get global render information\n";
    }
    
    return renderListOfLayoutsPlugin;
}

RenderLayoutPlugin* getRenderLayoutPlugin(SBasePlugin* renderBase) {
    RenderLayoutPlugin* renderLayoutPlugin = NULL;
    try {
        renderLayoutPlugin = dynamic_cast<RenderLayoutPlugin*>(renderBase); }
    catch(std::bad_cast) {
        std::cerr << "Unable to get local render information\n";
    }
    
    return renderLayoutPlugin;
}

void enableRenderPlugin(SBMLDocument* document) {
    if (document->getLevel() == 2)
        document->enablePackage(RenderExtension::getXmlnsL2(), "render",  true);
    else if (document->getLevel() == 3)
        document->enablePackage(RenderExtension::getXmlnsL3V1V1(), "render", true);
    document->setPackageRequired("render", false);
}

void enableRenderPlugin(Layout* layout) {
    if (layout->getLevel() == 2)
        layout->enablePackage(RenderExtension::getXmlnsL2(), "render",  true);
    else if (layout->getLevel() == 3)
        layout->enablePackage(RenderExtension::getXmlnsL3V1V1(), "render", true);
}

void addStyles(Layout* layout, LocalRenderInformation* localRenderInformation, RenderPkgNamespaces* renderPkgNamespaces) {
    addCompartmentGlyphsStyles(layout, localRenderInformation, renderPkgNamespaces);
    addSpeciesGlyphsStyles(layout, localRenderInformation, renderPkgNamespaces);
    addReactionGlyphsStyles(layout, localRenderInformation, renderPkgNamespaces);
}

Style* findStyle(LocalRenderInformation* localRenderInformation, const std::string& objectIdOrRole) {
    if (localRenderInformation) {
        for (unsigned int i = 0; i < localRenderInformation->getNumStyles(); i++) {
            std::set<std::string> idList = localRenderInformation->getStyle(i)->getIdList();
            if (idList.find(objectIdOrRole) != idList.end())
                return localRenderInformation->getStyle(i);
        }
        for (unsigned int i = 0; i < localRenderInformation->getNumStyles(); i++) {
            std::set<std::string> roleList = localRenderInformation->getStyle(i)->getRoleList();
            if (roleList.find(objectIdOrRole) != roleList.end())
                return localRenderInformation->getStyle(i);
        }
    }
    
    return NULL;
}

Style* findStyle(GlobalRenderInformation* globalRenderInformation, const std::string& objectRoleOrType) {
    if (globalRenderInformation) {
        Style* style = NULL;
        for (unsigned int i = 0; i < globalRenderInformation->getNumStyles(); i++) {
            // role
            std::set<std::string> roleList = globalRenderInformation->getStyle(i)->getRoleList();
            if (roleList.find(objectRoleOrType) != roleList.end())
                return globalRenderInformation->getStyle(i);
            // type
            std::set<std::string> typeList = globalRenderInformation->getStyle(i)->getTypeList();
            if (typeList.find(objectRoleOrType) != typeList.end())
                return globalRenderInformation->getStyle(i);
        }
    }
    
    return NULL;
}

void addCompartmentGlyphsStyles(Layout* layout, LocalRenderInformation* localRenderInformation, RenderPkgNamespaces* renderPkgNamespaces) {
    for (unsigned int i = 0; i < layout->getNumCompartmentGlyphs(); i++) {
        addCompartmentGlyphStyle(layout->getCompartmentGlyph(i), localRenderInformation, renderPkgNamespaces);
        addCompartmentTextGlyphsStyles(layout, localRenderInformation, layout->getCompartmentGlyph(i), renderPkgNamespaces);
    }
}

void addCompartmentGlyphStyle(CompartmentGlyph* compartmentGlyph, LocalRenderInformation* localRenderInformation, RenderPkgNamespaces* renderPkgNamespaces) {
    LocalStyle* localStyle = createLocalStyle(localRenderInformation, compartmentGlyph);
    setCompartmentGlyphRenderGroupFeatures(localStyle->createGroup(), renderPkgNamespaces);
}

void setCompartmentGlyphRenderGroupFeatures(RenderGroup* renderGroup, RenderPkgNamespaces* renderPkgNamespaces) {
    Rectangle* rectangle = renderGroup->createRectangle();
    setDefaultRectangleShapeFeatures(rectangle);
    rectangle->setStroke("darkcyan");
    rectangle->setStrokeWidth(2.0);
    rectangle->setFill("lightgray");
    rectangle->setRX(RelAbsVector(0.0, 5.0));
    rectangle->setRY(RelAbsVector(0.0, 5.0));
}

void addCompartmentTextGlyphsStyles(Layout* layout, LocalRenderInformation* localRenderInformation, CompartmentGlyph* compartmentGlyph, RenderPkgNamespaces* renderPkgNamespaces) {
    for (unsigned int i = 0; i < layout->getNumTextGlyphs(); i++) {
        if (layout->getTextGlyph(i)->getGraphicalObjectId() == compartmentGlyph->getId())
            addCompartmentTextGlyphStyle(layout->getTextGlyph(i), localRenderInformation, renderPkgNamespaces);
    }
}

void addCompartmentTextGlyphStyle(TextGlyph* textGlyph, LocalRenderInformation* localRenderInformation, RenderPkgNamespaces* renderPkgNamespaces) {
    LocalStyle* localStyle = createLocalStyle(localRenderInformation, textGlyph);
    setCompartmentTextGlyphRenderGroupFeatures(localStyle->createGroup(), renderPkgNamespaces);
}

void setCompartmentTextGlyphRenderGroupFeatures(RenderGroup* renderGroup, RenderPkgNamespaces* renderPkgNamespaces) {
    setGeneralTextGlyphRenderGroupFeatures(renderGroup, renderPkgNamespaces);
    renderGroup->setStroke("darkcyan");
    renderGroup->setFontSize(RelAbsVector(8.0, 0.0));
    renderGroup->setTextAnchor("start");
    renderGroup->setVTextAnchor("bottom");
}

void addSpeciesGlyphsStyles(Layout* layout, LocalRenderInformation* localRenderInformation, RenderPkgNamespaces* renderPkgNamespaces) {
    for (unsigned int i = 0; i < layout->getNumSpeciesGlyphs(); i++) {
        addSpeciesGlyphStyle(layout->getSpeciesGlyph(i), localRenderInformation, renderPkgNamespaces);
        addSpeciesTextGlyphsStyles(layout, localRenderInformation, layout->getSpeciesGlyph(i), renderPkgNamespaces);
    }
}

void addSpeciesGlyphStyle(SpeciesGlyph* speciesGlyph, LocalRenderInformation* localRenderInformation, RenderPkgNamespaces* renderPkgNamespaces) {
    LocalStyle* localStyle = createLocalStyle(localRenderInformation, speciesGlyph);
    setSpeciesGlyphRenderGroupFeatures(localStyle->createGroup(), renderPkgNamespaces);
}

void setSpeciesGlyphRenderGroupFeatures(RenderGroup* renderGroup, RenderPkgNamespaces* renderPkgNamespaces) {
    Rectangle* rectangle = renderGroup->createRectangle();
    setDefaultRectangleShapeFeatures(rectangle);
    rectangle->setStroke("black");
    rectangle->setStrokeWidth(2.0);
    rectangle->setFill("white");
}

void addSpeciesTextGlyphsStyles(Layout* layout, LocalRenderInformation* localRenderInformation, SpeciesGlyph* speciesGlyph, RenderPkgNamespaces* renderPkgNamespaces) {
    for (unsigned int i = 0; i < layout->getNumTextGlyphs(); i++) {
        if (layout->getTextGlyph(i)->getGraphicalObjectId() == speciesGlyph->getId())
            addSpeciesTextGlyphStyle(layout->getTextGlyph(i), localRenderInformation, renderPkgNamespaces);
    }
}

void addSpeciesTextGlyphStyle(TextGlyph* textGlyph, LocalRenderInformation* localRenderInformation, RenderPkgNamespaces* renderPkgNamespaces) {
    LocalStyle* localStyle = createLocalStyle(localRenderInformation, textGlyph);
    setSpeciesTextGlyphRenderGroupFeatures(localStyle->createGroup(), renderPkgNamespaces);
}

void setSpeciesTextGlyphRenderGroupFeatures(RenderGroup* renderGroup, RenderPkgNamespaces* renderPkgNamespaces) {
    setGeneralTextGlyphRenderGroupFeatures(renderGroup, renderPkgNamespaces);
    renderGroup->setFontSize(RelAbsVector(24.0, 0.0));
}

void addReactionGlyphsStyles(Layout* layout, LocalRenderInformation* localRenderInformation, RenderPkgNamespaces* renderPkgNamespaces) {
    for (unsigned int i = 0; i < layout->getNumReactionGlyphs(); i++) {
        addReactionGlyphStyle(layout->getReactionGlyph(i), localRenderInformation, renderPkgNamespaces);
        addSpeciesReferenceGlyphsStyles(layout->getReactionGlyph(i), localRenderInformation, renderPkgNamespaces);
    }
}

void addReactionGlyphStyle(ReactionGlyph* reactionGlyph, LocalRenderInformation* localRenderInformation, RenderPkgNamespaces* renderPkgNamespaces) {
    LocalStyle* localStyle = createLocalStyle(localRenderInformation, reactionGlyph);
    setReactionGlyphRenderGroupFeatures(localStyle->createGroup(), renderPkgNamespaces);
}

void setReactionGlyphRenderGroupFeatures(RenderGroup* renderGroup, RenderPkgNamespaces* renderPkgNamespaces) {
    Ellipse* ellipse = renderGroup->createEllipse();
    setDefaultEllipseShapeFeatures(ellipse);
    ellipse->setStroke("black");
    ellipse->setStrokeWidth(2.0);
    ellipse->setFill("white");
}

void addSpeciesReferenceGlyphsStyles(ReactionGlyph* reactionGlyph, LocalRenderInformation* localRenderInformation, RenderPkgNamespaces* renderPkgNamespaces) {
    for (int i = 0; i < reactionGlyph->getNumSpeciesReferenceGlyphs(); i++) {
        LocalStyle* localStyle = createLocalStyle(localRenderInformation, reactionGlyph->getSpeciesReferenceGlyph(i));
        setSpeciesReferenceGlyphRenderGroupFeatures(localStyle->createGroup(), reactionGlyph->getSpeciesReferenceGlyph(i)->getRole(), renderPkgNamespaces);
    }
}

void addSpeciesReferenceGlyphStyle(SpeciesReferenceGlyph* speciesReferenceGlyph, LocalRenderInformation* localRenderInformation, RenderPkgNamespaces* renderPkgNamespaces) {
    LocalStyle* localStyle = createLocalStyle(localRenderInformation, speciesReferenceGlyph);
    setSpeciesReferenceGlyphRenderGroupFeatures(localStyle->createGroup(), speciesReferenceGlyph->getRole(), renderPkgNamespaces);
}

void setSpeciesReferenceGlyphRenderGroupFeatures(RenderGroup* renderGroup, SpeciesReferenceRole_t role, RenderPkgNamespaces* renderPkgNamespaces) {
    renderGroup->setStroke("black");
    renderGroup->setStrokeWidth(2.0);
    
    if (role == SPECIES_ROLE_PRODUCT || role == SPECIES_ROLE_SIDEPRODUCT)
        renderGroup->setEndHead("productHead");
    else if (role == SPECIES_ROLE_MODIFIER)
        renderGroup->setEndHead("modifierHead");
    else if (role == SPECIES_ROLE_ACTIVATOR)
        renderGroup->setEndHead("activatorHead");
    else if (role == SPECIES_ROLE_INHIBITOR)
        renderGroup->setEndHead("inhibitorHead");
}

LocalStyle* createLocalStyle(LocalRenderInformation* localRenderInformation, GraphicalObject* graphicalObject) {
    LocalStyle* localStyle = localRenderInformation->createLocalStyle();
    localStyle->setId(graphicalObject->getId() + "_style");
    localStyle->addId(graphicalObject->getId());
    return localStyle;
}

void setGeneralTextGlyphRenderGroupFeatures(RenderGroup* renderGroup, RenderPkgNamespaces* renderPkgNamespaces) {
    renderGroup->setStroke("black");
    renderGroup->setFontSize(RelAbsVector(24.0, 0.0));
    renderGroup->setFontFamily("monospace");
    renderGroup->setFontWeight("normal");
    renderGroup->setFontStyle("normal");
    renderGroup->setTextAnchor("middle");
    renderGroup->setVTextAnchor("middle");
}

void addDefaultColors(GlobalRenderInformation* globalRenderInformation, RenderPkgNamespaces* renderPkgNamespaces) {
    addWhiteColor(globalRenderInformation, renderPkgNamespaces);
    addBlackColor(globalRenderInformation, renderPkgNamespaces);
    addLightGrayColor(globalRenderInformation, renderPkgNamespaces);
    addDarkCyanColor(globalRenderInformation, renderPkgNamespaces);
    addTealColor(globalRenderInformation, renderPkgNamespaces);
    addSilverColor(globalRenderInformation, renderPkgNamespaces);
}

void addWhiteColor(GlobalRenderInformation* globalRenderInformation, RenderPkgNamespaces* renderPkgNamespaces) {
    if (!globalRenderInformation->getColorDefinition("white"))
        globalRenderInformation->addColorDefinition(createColorDefintion(renderPkgNamespaces, "white", 255, 255, 255));
}

void addBlackColor(GlobalRenderInformation* globalRenderInformation, RenderPkgNamespaces* renderPkgNamespaces) {
    if (!globalRenderInformation->getColorDefinition("black"))
        globalRenderInformation->addColorDefinition(createColorDefintion(renderPkgNamespaces, "black", 0, 0, 0));
}

void addLightGrayColor(GlobalRenderInformation* globalRenderInformation, RenderPkgNamespaces* renderPkgNamespaces) {
    if (!globalRenderInformation->getColorDefinition("lightgray"))
        globalRenderInformation->addColorDefinition(createColorDefintion(renderPkgNamespaces, "lightgray", 211, 211, 211));
}

void addDarkCyanColor(GlobalRenderInformation* globalRenderInformation, RenderPkgNamespaces* renderPkgNamespaces) {
    if (!globalRenderInformation->getColorDefinition("darkcyan"))
        globalRenderInformation->addColorDefinition(createColorDefintion(renderPkgNamespaces, "darkcyan", 0, 139, 139));
}

void addTealColor(GlobalRenderInformation* globalRenderInformation, RenderPkgNamespaces* renderPkgNamespaces) {
    if (!globalRenderInformation->getColorDefinition("teal"))
        globalRenderInformation->addColorDefinition(createColorDefintion(renderPkgNamespaces, "teal", 0, 128, 128));
}

void addSilverColor(GlobalRenderInformation* globalRenderInformation, RenderPkgNamespaces* renderPkgNamespaces) {
    if (!globalRenderInformation->getColorDefinition("silver"))
        globalRenderInformation->addColorDefinition(createColorDefintion(renderPkgNamespaces, "silver", 192, 192, 192));
}

ColorDefinition* createColorDefintion(RenderPkgNamespaces* renderPkgNamespaces, const std::string &id, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    return new ColorDefinition(renderPkgNamespaces, id, r, g, b, a);
}

void addDefaultLineEndings(GlobalRenderInformation* globalRenderInformation, LayoutPkgNamespaces* layoutPkgNamespaces, RenderPkgNamespaces* renderPkgNamespaces) {
    addProductHeadLineEnding(globalRenderInformation, layoutPkgNamespaces, renderPkgNamespaces);
    addModifierHeadLineEnding(globalRenderInformation, layoutPkgNamespaces, renderPkgNamespaces);
    addActivatorHeadLineEnding(globalRenderInformation, layoutPkgNamespaces, renderPkgNamespaces);
    addInhibitorHeadLineEnding(globalRenderInformation, layoutPkgNamespaces, renderPkgNamespaces);
}

void addProductHeadLineEnding(GlobalRenderInformation* globalRenderInformation, LayoutPkgNamespaces* layoutPkgNamespaces, RenderPkgNamespaces* renderPkgNamespaces) {
    if (!globalRenderInformation->getLineEnding("productHead"))
        globalRenderInformation->addLineEnding(createProductHeadLineEnding(layoutPkgNamespaces, renderPkgNamespaces));
}

LineEnding* createProductHeadLineEnding(LayoutPkgNamespaces* layoutPkgNamespaces, RenderPkgNamespaces* renderPkgNamespaces) {
    LineEnding* lineEnding = new LineEnding(renderPkgNamespaces, "productHead");
    setLineEndingGeneralFeatures(lineEnding, layoutPkgNamespaces);
    setProductHeadLineEndingExcluseFeatures(lineEnding, renderPkgNamespaces);
    return lineEnding;
}

void setProductHeadLineEndingExcluseFeatures(LineEnding* lineEnding, RenderPkgNamespaces* renderPkgNamespaces) {
    RenderGroup* renderGroup = lineEnding->getGroup();
    Polygon* triangle = renderGroup->createPolygon();
    setDefaultTriangleShapeFeatures(triangle);
    triangle->getElement(1)->setY(RelAbsVector(0.0, 50.0));
    triangle->getElement(2)->setX(RelAbsVector(0.0, 0.0));
    triangle->setStroke("black");
    triangle->setStrokeWidth(2.0);
    triangle->setFill("black");
}

void addModifierHeadLineEnding(GlobalRenderInformation* globalRenderInformation, LayoutPkgNamespaces* layoutPkgNamespaces, RenderPkgNamespaces* renderPkgNamespaces) {
    if (!globalRenderInformation->getLineEnding("modifierHead"))
        globalRenderInformation->addLineEnding(createModifierHeadLineEnding(layoutPkgNamespaces, renderPkgNamespaces));
}

LineEnding* createModifierHeadLineEnding(LayoutPkgNamespaces* layoutPkgNamespaces, RenderPkgNamespaces* renderPkgNamespaces) {
    LineEnding* lineEnding = new LineEnding(renderPkgNamespaces, "modifierHead");
    setLineEndingGeneralFeatures(lineEnding, layoutPkgNamespaces);
    setModifierHeadLineEndingExcluseFeatures(lineEnding, renderPkgNamespaces);
    return lineEnding;
}

void setModifierHeadLineEndingExcluseFeatures(LineEnding* lineEnding, RenderPkgNamespaces* renderPkgNamespaces) {
    RenderGroup* renderGroup = lineEnding->getGroup();
    Polygon* diamond = renderGroup->createPolygon();
    setDefaultDiamondShapeFeatures(diamond);
    diamond->setStroke("black");
    diamond->setStrokeWidth(2.0);
    diamond->setFill("white");
}

void addActivatorHeadLineEnding(GlobalRenderInformation* globalRenderInformation, LayoutPkgNamespaces* layoutPkgNamespaces, RenderPkgNamespaces* renderPkgNamespaces) {
    if (!globalRenderInformation->getLineEnding("activatorHead"))
        globalRenderInformation->addLineEnding(createActivatorHeadLineEnding(layoutPkgNamespaces, renderPkgNamespaces));
}

LineEnding* createActivatorHeadLineEnding(LayoutPkgNamespaces* layoutPkgNamespaces, RenderPkgNamespaces* renderPkgNamespaces) {
    LineEnding* lineEnding = new LineEnding(renderPkgNamespaces, "activatorHead");
    setLineEndingGeneralFeatures(lineEnding, layoutPkgNamespaces);
    setActivatorHeadLineEndingExcluseFeatures(lineEnding, renderPkgNamespaces);
    return lineEnding;
}

void setActivatorHeadLineEndingExcluseFeatures(LineEnding* lineEnding, RenderPkgNamespaces* renderPkgNamespaces) {
    RenderGroup* renderGroup = lineEnding->getGroup();
    Ellipse* ellipse = renderGroup->createEllipse();
    setDefaultEllipseShapeFeatures(ellipse);
    ellipse->setStroke("black");
    ellipse->setStrokeWidth(2.0);
    ellipse->setFill("white");
}

void addInhibitorHeadLineEnding(GlobalRenderInformation* globalRenderInformation, LayoutPkgNamespaces* layoutPkgNamespaces, RenderPkgNamespaces* renderPkgNamespaces) {
    if (!globalRenderInformation->getLineEnding("inhibitorHead"))
        globalRenderInformation->addLineEnding(createInhibitorHeadLineEnding(layoutPkgNamespaces, renderPkgNamespaces));
}

LineEnding* createInhibitorHeadLineEnding(LayoutPkgNamespaces* layoutPkgNamespaces, RenderPkgNamespaces* renderPkgNamespaces) {
    LineEnding* lineEnding = new LineEnding(renderPkgNamespaces, "inhibitorHead");
    lineEnding->createGroup();
    setLineEndingGeneralFeatures(lineEnding, layoutPkgNamespaces);
    setInhibitorHeadLineEndingExcluseFeatures(lineEnding, renderPkgNamespaces);
    return lineEnding;
}

void setInhibitorHeadLineEndingExcluseFeatures(LineEnding* lineEnding, RenderPkgNamespaces* renderPkgNamespaces) {
    RenderGroup* renderGroup = lineEnding->getGroup();
    Rectangle* rectangle = renderGroup->createRectangle();
    setDefaultRectangleShapeFeatures(rectangle);
    rectangle->setX(RelAbsVector(0.0, 80.0));
    rectangle->setWidth(RelAbsVector(0.0, 20.0));
    rectangle->setRX(RelAbsVector(0.0, 0.0));
    rectangle->setRY(RelAbsVector(0.0, 0.0));
    rectangle->setStroke("black");
    rectangle->setStrokeWidth(2.0);
    rectangle->setFill("black");
}

void setLineEndingGeneralFeatures(LineEnding* lineEnding, LayoutPkgNamespaces* layoutPkgNamespaces) {
    lineEnding->setEnableRotationalMapping(true);
    lineEnding->setBoundingBox(new BoundingBox(layoutPkgNamespaces, lineEnding->getId() + "_bb", -12.0, -6.0, 12.0, 12.0));
}

void setDefaultRectangleShapeFeatures(Rectangle* rectangle) {
    rectangle->setX(RelAbsVector(0.0, 0.0));
    rectangle->setY(RelAbsVector(0.0, 0.0));
    rectangle->setWidth(RelAbsVector(0.0, 100.0));
    rectangle->setHeight(RelAbsVector(0.0, 100.0));
    rectangle->setRX(RelAbsVector(0.0, 10.0));
    rectangle->setRY(RelAbsVector(0.0, 10.0));
}

void setDefaultEllipseShapeFeatures(Ellipse* ellipse) {
    ellipse->setCX(RelAbsVector(0.0, 50.0));
    ellipse->setCY(RelAbsVector(0.0, 50.0));
    ellipse->setRX(RelAbsVector(0.0, 50.0));
    ellipse->setRY(RelAbsVector(0.0, 50.0));
}

void setDefaultTriangleShapeFeatures(Polygon* triangle) {
    RenderPoint* point = NULL;
    point = triangle->createPoint();
    point->setX(RelAbsVector(0.0, 0.0));
    point->setY(RelAbsVector(0.0, 0.0));
    point = triangle->createPoint();
    point->setX(RelAbsVector(0.0, 100.0));
    point->setY(RelAbsVector(0.0, 0.0));
    point = triangle->createPoint();
    point->setX(RelAbsVector(0.0, 50.0));
    point->setY(RelAbsVector(0.0, 100.0));
}

void setDefaultDiamondShapeFeatures(Polygon* diamond) {
    RenderPoint* point = NULL;
    point = diamond->createPoint();
    point->setX(RelAbsVector(0.0, 0.0));
    point->setY(RelAbsVector(0.0, 50.0));
    point = diamond->createPoint();
    point->setX(RelAbsVector(0.0, 50.0));
    point->setY(RelAbsVector(0.0, 0.0));
    point = diamond->createPoint();
    point->setX(RelAbsVector(0.0, 100.0));
    point->setY(RelAbsVector(0.0, 50.0));
    point = diamond->createPoint();
    point->setX(RelAbsVector(0.0, 50.0));
    point->setY(RelAbsVector(0.0, 100.0));
}

void setDefaultPentagonShapeFeatures(Polygon* pentagon) {
    const double pi = 3.14159265;
    RenderPoint* point = NULL;
    point = pentagon->createPoint();
    point->setX(RelAbsVector(0.0, 50. * (1 - std::sin(0.4 * pi))));
    point->setY(RelAbsVector(0.0, 50. * (1 - std::cos(0.4 * pi))));
    point = pentagon->createPoint();
    point->setX(RelAbsVector(0.0, 50.));
    point->setY(RelAbsVector(0.0, 0.));
    point = pentagon->createPoint();
    point->setX(RelAbsVector(0.0, 50. * (1 + std::sin(0.4 * pi))));
    point->setY(RelAbsVector(0.0, 50. * (1 - std::cos(0.4 * pi))));
    point = pentagon->createPoint();
    point->setX(RelAbsVector(0.0, 50. * (1 + std::sin(0.2 * pi))));
    point->setY(RelAbsVector(0.0, 50. * (1 + std::cos(0.2 * pi))));
    point = pentagon->createPoint();
    point->setX(RelAbsVector(0.0, 50. * (1 - std::sin(0.2 * pi))));
    point->setY(RelAbsVector(0.0, 50. * (1 + std::cos(0.2 * pi))));
}

void setDefaultHexagonShapeFeatures(Polygon* hexagon) {
    RenderPoint* point = NULL;
    point = hexagon->createPoint();
    point->setX(RelAbsVector(0.0, 0.0));
    point->setY(RelAbsVector(0.0, 25.0));
    point = hexagon->createPoint();
    point->setX(RelAbsVector(0.0, 50.0));
    point->setY(RelAbsVector(0.0, 0.0));
    point = hexagon->createPoint();
    point->setX(RelAbsVector(0.0, 100.0));
    point->setY(RelAbsVector(0.0, 25.0));
    point = hexagon->createPoint();
    point->setX(RelAbsVector(0.0, 100.0));
    point->setY(RelAbsVector(0.0, 75.0));
    point = hexagon->createPoint();
    point->setX(RelAbsVector(0.0, 50.0));
    point->setY(RelAbsVector(0.0, 100.0));
    point = hexagon->createPoint();
    point->setX(RelAbsVector(0.0, 0.0));
    point->setY(RelAbsVector(0.0, 75.0));
}

void setDefaultOctagonShapeFeatures(Polygon* octagon) {
    const double pi = 3.14159265;
    RenderPoint* point = NULL;
    point = octagon->createPoint();
    point->setX(RelAbsVector(0.0, 0.0));
    point->setY(RelAbsVector(0.0, 50.0));
    point = octagon->createPoint();
    point->setX(RelAbsVector(0.0, 50. * (1 - std::cos(pi / 4.0))));
    point->setY(RelAbsVector(0.0, 50. * (1 - std::sin(pi / 4.0))));
    point = octagon->createPoint();
    point->setX(RelAbsVector(0.0, 50.0));
    point->setY(RelAbsVector(0.0, 0.0));
    point = octagon->createPoint();
    point->setX(RelAbsVector(0.0, 50. * (1 + std::cos(pi / 4.0))));
    point->setY(RelAbsVector(0.0, 50. * (1 - std::sin(pi / 4.0))));
    point = octagon->createPoint();
    point->setX(RelAbsVector(0.0, 100.0));
    point->setY(RelAbsVector(0.0, 50.0));
    point = octagon->createPoint();
    point->setX(RelAbsVector(0.0, 50. * (1 + std::cos(pi / 4.0))));
    point->setY(RelAbsVector(0.0, 50. * (1 + std::sin(pi / 4.0))));
    point = octagon->createPoint();
    point->setX(RelAbsVector(0.0, 50.0));
    point->setY(RelAbsVector(0.0, 100.0));
    point = octagon->createPoint();
    point->setX(RelAbsVector(0.0, 50. * (1 - std::cos(pi / 4.0))));
    point->setY(RelAbsVector(0.0, 50. * (1 + std::sin(pi / 4.0))));
}

void setDefaultRenderCurveShapeFeatures(RenderCurve* renderCurve) {
    RenderPoint* point = NULL;
    point = renderCurve->createPoint();
    point->setX(RelAbsVector(0.0, 0.0));
    point->setY(RelAbsVector(0.0, 50.0));
    point = renderCurve->createPoint();
    point->setX(RelAbsVector(0.0, 100.0));
    point->setY(RelAbsVector(0.0, 50.0));
}

void setDefaultImageShapeFeatures(Image* image) {
    image->setX(RelAbsVector(0.0, 0.0));
    image->setY(RelAbsVector(0.0, 0.0));
    image->setWidth(RelAbsVector(0.0, 100.0));
    image->setHeight(RelAbsVector(0.0, 100.0));
}

}
