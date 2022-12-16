#include "libsbml_ne_layout_helpers.h"

namespace LIBSBML_NETWORKEDITOR_CPP_NAMESPACE  {

LayoutModelPlugin* getLayoutModelPlugin(SBMLDocument* document) {
    SBasePlugin* layoutBase = getLayoutPlugin(document);
    if (layoutBase)
        return getLayoutModelPlugin(layoutBase);
    
    return NULL;
}

LayoutModelPlugin* enableAndGetLayoutModelPlugin(SBMLDocument* document) {
    SBasePlugin* layoutBase = getLayoutPlugin(document);
    if (!layoutBase) {
        enableLayoutPlugin(document);
        layoutBase = getLayoutPlugin(document);
    }
    if (layoutBase)
        return getLayoutModelPlugin(layoutBase);
    
    return NULL;
}

SBasePlugin* getLayoutPlugin(SBMLDocument* document) {
    Model* model = document->getModel();
    if (model)
        return model->getPlugin("layout");
    else
        std::cerr << "Failed to load model\n";
    
    return NULL;
}

LayoutModelPlugin* getLayoutModelPlugin(SBasePlugin* layoutBase) {
    LayoutModelPlugin* layoutModelPlugin = NULL;
    try {
        layoutModelPlugin = dynamic_cast<LayoutModelPlugin*>(layoutBase); }
    catch(std::bad_cast) {
        std::cerr << "Unable to get layout information\n"; }
    
    return layoutModelPlugin;
}

void enableLayoutPlugin(SBMLDocument* document) {
    if (document->getLevel() == 2)
        document->enablePackage(LayoutExtension::getXmlnsL2(), "layout",  true);
    else if (document->getLevel() == 3)
        document->enablePackage(LayoutExtension::getXmlnsL3V1V1(), "layout",  true);
    document->setPackageRequired("layout", false);
}

void setCompartmentGlyphs(Model* model, Layout* layout, LayoutPkgNamespaces* layoutPkgNamespaces) {
    for (unsigned int i = 0; i < model->getNumCompartments(); i++) {
        Compartment* compartment = model->getCompartment(i);
        CompartmentGlyph* compartmentGlyph = getCompartmentGlyph(layout, compartment);
        setGraphicalObjectBoundingBox(compartmentGlyph, layoutPkgNamespaces);
    }
}

void setSpeciesGlyphs(Model* model, Layout* layout, LayoutPkgNamespaces* layoutPkgNamespaces) {
    for (unsigned int i = 0; i < model->getNumSpecies(); i++) {
        Species* species = model->getSpecies(i);
        SpeciesGlyph* speciesGlyph = getSpeciesGlyph(layout, species);
        setGraphicalObjectBoundingBox(speciesGlyph, layoutPkgNamespaces);
    }
}

void setReactionGlyphs(Model* model, Layout* layout, LayoutPkgNamespaces* layoutPkgNamespaces) {
    for (unsigned int i = 0; i < model->getNumReactions(); i++) {
        Reaction* reaction = model->getReaction(i);
        ReactionGlyph* reactionGlyph = getReactionGlyph(layout, reaction);
        setGraphicalObjectBoundingBox(reactionGlyph, layoutPkgNamespaces);
        setReactantGlyphs(layout, reaction, reactionGlyph, layoutPkgNamespaces);
        setProductGlyphs(layout, reaction, reactionGlyph, layoutPkgNamespaces);
        setModifierGlyphs(layout, reaction, reactionGlyph, layoutPkgNamespaces);
    }
}

void setReactantGlyphs(Layout* layout, Reaction* reaction, ReactionGlyph* reactionGlyph, LayoutPkgNamespaces* layoutPkgNamespaces) {
    for (unsigned int i = 0; i < reaction->getNumReactants(); i++) {
        SimpleSpeciesReference* speciesReference = reaction->getReactant(i);
        SpeciesReferenceGlyph* speciesReferenceGlyph = getSpeciesReferenceGlyph(layout, reactionGlyph, speciesReference);
        speciesReferenceGlyph->setRole(SPECIES_ROLE_SUBSTRATE);
        setSpeciesReferenceGlyphCurve(speciesReferenceGlyph, layoutPkgNamespaces);
    }
}

void setProductGlyphs(Layout* layout, Reaction* reaction, ReactionGlyph* reactionGlyph, LayoutPkgNamespaces* layoutPkgNamespaces) {
    for (unsigned int i = 0; i < reaction->getNumProducts(); i++) {
        SimpleSpeciesReference* speciesReference = reaction->getProduct(i);
        SpeciesReferenceGlyph* speciesReferenceGlyph = getSpeciesReferenceGlyph(layout, reactionGlyph, speciesReference);
        speciesReferenceGlyph->setRole(SPECIES_ROLE_PRODUCT);
        setSpeciesReferenceGlyphCurve(speciesReferenceGlyph, layoutPkgNamespaces);
    }
}

void setModifierGlyphs(Layout* layout, Reaction* reaction, ReactionGlyph* reactionGlyph, LayoutPkgNamespaces* layoutPkgNamespaces) {
    for (unsigned int i = 0; i < reaction->getNumModifiers(); i++) {
        SimpleSpeciesReference* speciesReference = reaction->getModifier(i);
        SpeciesReferenceGlyph* speciesReferenceGlyph = getSpeciesReferenceGlyph(layout, reactionGlyph, speciesReference);
        speciesReferenceGlyph->setRole(SPECIES_ROLE_MODIFIER);
        setSpeciesReferenceGlyphCurve(speciesReferenceGlyph, layoutPkgNamespaces);
    }
}

void setCompartmentTextGlyphs(Layout* layout, LayoutPkgNamespaces* layoutPkgNamespaces) {
    for (unsigned int i = 0; i < layout->getNumCompartmentGlyphs(); i++) {
        TextGlyph* textGlyph = getTextGlyph(layout, layout->getCompartmentGlyph(i));
        setTextGlyphBoundingBox(textGlyph, layout->getCompartmentGlyph(i), layoutPkgNamespaces);
    }
}

void setSpeciesTextGlyphs(Layout* layout, LayoutPkgNamespaces* layoutPkgNamespaces) {
    for (unsigned int i = 0; i < layout->getNumSpeciesGlyphs(); i++) {
        TextGlyph* textGlyph = getTextGlyph(layout, layout->getSpeciesGlyph(i));
        setTextGlyphBoundingBox(textGlyph, layout->getSpeciesGlyph(i), layoutPkgNamespaces);
    }
}

CompartmentGlyph* getCompartmentGlyph(Layout* layout, Compartment* compartment) {
    for (unsigned int i = 0; i < layout->getNumCompartmentGlyphs(); i++) {
        if (compartmentGlyphBelongs(layout->getCompartmentGlyph(i), compartment))
            return layout->getCompartmentGlyph(i);
    }
    CompartmentGlyph* compartmentGlyph = layout->createCompartmentGlyph();
    compartmentGlyph->setId(compartment->getId() + "_Glyph_1");
    compartmentGlyph->setCompartmentId(compartment->getId());
    
    return compartmentGlyph;
}

SpeciesGlyph* getSpeciesGlyph(Layout* layout, Species* species) {
    for (unsigned int i = 0; i < layout->getNumSpeciesGlyphs(); i++) {
        if (speciesGlyphBelongs(layout->getSpeciesGlyph(i), species))
            return layout->getSpeciesGlyph(i);
    }
    SpeciesGlyph* speciesGlyph = layout->createSpeciesGlyph();
    speciesGlyph->setId(species->getId() + "_Glyph_1");
    speciesGlyph->setSpeciesId(species->getId());
    
    return speciesGlyph;
}

ReactionGlyph* getReactionGlyph(Layout* layout, Reaction* reaction) {
    for (unsigned int i = 0; i < layout->getNumReactionGlyphs(); i++) {
        if (reactionGlyphBelongs(layout->getReactionGlyph(i), reaction))
            return layout->getReactionGlyph(i);
    }
    ReactionGlyph* reactionGlyph = layout->createReactionGlyph();
    reactionGlyph->setId(reaction->getId() + "_Glyph_1");
    reactionGlyph->setReactionId(reaction->getId());
    
    return reactionGlyph;
}

SpeciesReferenceGlyph* getSpeciesReferenceGlyph(Layout* layout, ReactionGlyph* reactionGlyph, SimpleSpeciesReference* speciesReference) {
    for (unsigned int i = 0; i < reactionGlyph->getNumSpeciesReferenceGlyphs(); i++) {
        if (speciesReferenceGlyphBelongs(layout, reactionGlyph->getSpeciesReferenceGlyph(i), speciesReference))
            return reactionGlyph->getSpeciesReferenceGlyph(i);
    }
    SpeciesReferenceGlyph* speciesReferenceGlyph = reactionGlyph->createSpeciesReferenceGlyph();
    if (!speciesReference->getId().empty()) {
        speciesReferenceGlyph->setId(speciesReference->getId() + "_Glyph_1");
        speciesReferenceGlyph->setSpeciesReferenceId(speciesReference->getId());
    }
    else
        speciesReferenceGlyph->setId(speciesReference->getSpecies() + "_Glyph_1_" + reactionGlyph->getId() + "_SpeciesReference" + std::to_string(reactionGlyph->getNumSpeciesReferenceGlyphs()));
    speciesReferenceGlyph->setSpeciesGlyphId(speciesReference->getSpecies() + "_Glyph_1");
    
    return speciesReferenceGlyph;
}

TextGlyph* getTextGlyph(Layout* layout, GraphicalObject* graphicalObject) {
    for (unsigned int i = 0; i < layout->getNumTextGlyphs(); i++) {
        if (textGlyphBelongs(layout->getTextGlyph(i), graphicalObject))
            return layout->getTextGlyph(i);
    }
    TextGlyph* textGlyph = layout->createTextGlyph();
    textGlyph->setId(graphicalObject->getId() + "_TextGlyph_1");
    textGlyph->setGraphicalObjectId(graphicalObject->getId());
    textGlyph->setOriginOfTextId(getEntityId(layout, graphicalObject));
    
    return textGlyph;
}


void setGraphicalObjectBoundingBox(GraphicalObject* graphicalObject, LayoutPkgNamespaces* layoutPkgNamespaces) {
    graphicalObject->setBoundingBox(new BoundingBox(layoutPkgNamespaces, graphicalObject->getId() + "_bb", 0, 0, 0, 0));
}

void setTextGlyphBoundingBox(TextGlyph* textGlyph, GraphicalObject* graphicalObject, LayoutPkgNamespaces* layoutPkgNamespaces) {
    BoundingBox* box = graphicalObject->getBoundingBox();
    textGlyph->setBoundingBox(new BoundingBox(layoutPkgNamespaces, textGlyph->getId() + "_bb", box->x(), box->y(), box->width(), box->height()));
}

void setSpeciesReferenceGlyphCurve(SpeciesReferenceGlyph* speciesReferenceGlyph, LayoutPkgNamespaces* layoutPkgNamespaces) {
    if (!speciesReferenceGlyph->isSetCurve()) {
        Curve* curve = speciesReferenceGlyph->getCurve();
        CubicBezier* cubicBezier = curve->createCubicBezier();
        cubicBezier->setStart(new Point(layoutPkgNamespaces, 0, 0));
        cubicBezier->setBasePoint1(new Point(layoutPkgNamespaces, 0, 0));
        cubicBezier->setBasePoint2(new Point(layoutPkgNamespaces, 0, 0));
        cubicBezier->setEnd(new Point(layoutPkgNamespaces, 0, 0));
    }
}

Compartment* findSpeicesGlyphCompartment(Model* model, SpeciesGlyph* speciesGlyph) {
    if (model && speciesGlyph) {
        Species* species = model->getSpecies(speciesGlyph->getSpeciesId());
        if (species) {
            for (unsigned int i = 0; i < model->getNumCompartments(); i++) {
                if (model->getCompartment(i)->getId() == species->getCompartment())
                    return model->getCompartment(i);
            }
        }
    }
    
    return NULL;
}

bool containsSpecies(Model* model, Layout* layout, CompartmentGlyph* compartmentGlyph) {
    std::string compartmentId = getEntityId(layout, compartmentGlyph);
    for (unsigned int i = 0; i < model->getNumSpecies(); i++)
        if (model->getSpecies(i)->getCompartment() == compartmentId)
            return true;
    
    return false;
}

bool compartmentGlyphBelongs(CompartmentGlyph* compartmentGlyph, Compartment* compartment) {
    return compartmentGlyph->getCompartmentId() == compartment->getId() ? true : false;
}
    
bool speciesGlyphBelongs(SpeciesGlyph* speciesGlyph, Species* species) {
    return speciesGlyph->getSpeciesId() == species->getId() ? true : false;
}

bool reactionGlyphBelongs(ReactionGlyph* reactionGlyph, Reaction* reaction) {
    return reactionGlyph->getReactionId() == reaction->getId() ? true : false;
}

bool speciesReferenceGlyphBelongs(Layout* layout, SpeciesReferenceGlyph* speciesReferenceGlyph, SimpleSpeciesReference* speciesReference) {
    SpeciesGlyph* speciesGlyph = layout->getSpeciesGlyph(speciesReferenceGlyph->getSpeciesGlyphId());
    if (speciesGlyph)
        return speciesGlyph->getSpeciesId() == speciesReference->getSpecies() ? true : false;
    
    return false;
}

bool textGlyphBelongs(TextGlyph* textGlyph, GraphicalObject* graphicalObject) {
    return textGlyph->getGraphicalObjectId() == graphicalObject->getId() ? true : false;
}

const std::string getEntityId(Layout* layout, GraphicalObject* graphicalObject) {
    for (unsigned int i = 0; i < layout->getNumCompartmentGlyphs(); i++) {
        if (graphicalObject->getId() == layout->getCompartmentGlyph(i)->getId())
            return layout->getCompartmentGlyph(i)->getCompartmentId();
    }
    
    for (unsigned int i = 0; i < layout->getNumSpeciesGlyphs(); i++) {
        if (graphicalObject->getId() == layout->getSpeciesGlyph(i)->getId())
            return layout->getSpeciesGlyph(i)->getSpeciesId();
    }
    
    for (unsigned int i = 0; i < layout->getNumReactionGlyphs(); i++) {
        if (graphicalObject->getId() == layout->getReactionGlyph(i)->getId())
            return layout->getReactionGlyph(i)->getReactionId();
        
        ReactionGlyph* reactionGlyph = layout->getReactionGlyph(i);
        for (unsigned int j = 0; j < reactionGlyph->getNumSpeciesReferenceGlyphs(); j++) {
            if (graphicalObject->getId() == reactionGlyph->getSpeciesReferenceGlyph(j)->getId())
                return reactionGlyph->getSpeciesReferenceGlyph(i)->getSpeciesReferenceId();
        }
    }
    
    return "";
}

}
