<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="8.6.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="24" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="50" name="dxf" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="SeanSylwester">
<packages>
<package name="TO-236AB">
<smd name="3" x="1.25" y="2.35" dx="0.6" dy="0.7" layer="1"/>
<smd name="1" x="0.3" y="0.35" dx="0.6" dy="0.7" layer="1"/>
<smd name="2" x="2.2" y="0.35" dx="0.6" dy="0.7" layer="1"/>
<circle x="0.05" y="0.05" radius="0.05" width="0" layer="21"/>
<wire x1="-0.2" y1="2.2" x2="-0.2" y2="0.5" width="0.127" layer="51"/>
<wire x1="-0.2" y1="0.5" x2="2.7" y2="0.5" width="0.127" layer="51"/>
<wire x1="2.7" y1="0.5" x2="2.7" y2="2.2" width="0.127" layer="51"/>
<wire x1="2.7" y1="2.2" x2="-0.2" y2="2.2" width="0.127" layer="51"/>
<wire x1="0.9" y1="0.5" x2="1.6" y2="0.5" width="0.127" layer="21"/>
<wire x1="2.7" y1="0.75" x2="2.7" y2="1.95" width="0.127" layer="21"/>
<wire x1="-0.2" y1="0.75" x2="-0.2" y2="1.95" width="0.127" layer="21"/>
<wire x1="0.05" y1="2.2" x2="0.7" y2="2.2" width="0.127" layer="21"/>
<wire x1="1.8" y1="2.2" x2="2.45" y2="2.2" width="0.127" layer="21"/>
<text x="-0.3" y="2.9" size="0.6096" layer="25">&gt;NAME</text>
<text x="-0.4" y="-0.95" size="0.6096" layer="27">&gt;VALUE</text>
</package>
</packages>
<symbols>
<symbol name="2N7002P">
<pin name="GATE" x="-40.64" y="15.24" length="middle"/>
<pin name="SOURCE" x="-40.64" y="0" length="middle"/>
<pin name="DRAIN" x="-17.78" y="7.62" length="middle" rot="R180"/>
<wire x1="-35.56" y1="20.32" x2="-35.56" y2="-5.08" width="0.254" layer="94"/>
<wire x1="-35.56" y1="-5.08" x2="-22.86" y2="-5.08" width="0.254" layer="94"/>
<wire x1="-22.86" y1="-5.08" x2="-22.86" y2="20.32" width="0.254" layer="94"/>
<wire x1="-22.86" y1="20.32" x2="-35.56" y2="20.32" width="0.254" layer="94"/>
<text x="-33.02" y="20.32" size="1.778" layer="95">&gt;NAME</text>
<text x="-33.02" y="-7.62" size="1.778" layer="96">&gt;VALUE</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="2N7002P" prefix="M">
<gates>
<gate name="G$1" symbol="2N7002P" x="40.64" y="7.62"/>
</gates>
<devices>
<device name="" package="TO-236AB">
<connects>
<connect gate="G$1" pin="DRAIN" pad="3"/>
<connect gate="G$1" pin="GATE" pad="1"/>
<connect gate="G$1" pin="SOURCE" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="wirepad" urn="urn:adsk.eagle:library:412">
<description>&lt;b&gt;Single Pads&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="SMD1,27-2,54" urn="urn:adsk.eagle:footprint:30822/1" library_version="1">
<description>&lt;b&gt;SMD PAD&lt;/b&gt;</description>
<smd name="1" x="0" y="0" dx="1.27" dy="2.54" layer="1"/>
<text x="0" y="0" size="0.0254" layer="27">&gt;VALUE</text>
<text x="-0.8" y="-2.4" size="1.27" layer="25" rot="R90">&gt;NAME</text>
</package>
</packages>
<packages3d>
<package3d name="SMD1,27-2,54" urn="urn:adsk.eagle:package:30839/1" type="box" library_version="1">
<description>SMD PAD</description>
</package3d>
</packages3d>
<symbols>
<symbol name="PAD" urn="urn:adsk.eagle:symbol:30808/1" library_version="1">
<wire x1="-1.016" y1="1.016" x2="1.016" y2="-1.016" width="0.254" layer="94"/>
<wire x1="-1.016" y1="-1.016" x2="1.016" y2="1.016" width="0.254" layer="94"/>
<text x="-1.143" y="1.8542" size="1.778" layer="95">&gt;NAME</text>
<text x="-1.143" y="-3.302" size="1.778" layer="96">&gt;VALUE</text>
<pin name="P" x="2.54" y="0" visible="off" length="short" direction="pas" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="SMD2" urn="urn:adsk.eagle:component:30857/1" prefix="PAD" uservalue="yes" library_version="1">
<description>&lt;b&gt;SMD PAD&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="PAD" x="0" y="0"/>
</gates>
<devices>
<device name="" package="SMD1,27-2,54">
<connects>
<connect gate="1" pin="P" pad="1"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:30839/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="M1" library="SeanSylwester" deviceset="2N7002P" device=""/>
<part name="PAD1" library="wirepad" library_urn="urn:adsk.eagle:library:412" deviceset="SMD2" device="" package3d_urn="urn:adsk.eagle:package:30839/1"/>
<part name="PAD2" library="wirepad" library_urn="urn:adsk.eagle:library:412" deviceset="SMD2" device="" package3d_urn="urn:adsk.eagle:package:30839/1"/>
<part name="PAD3" library="wirepad" library_urn="urn:adsk.eagle:library:412" deviceset="SMD2" device="" package3d_urn="urn:adsk.eagle:package:30839/1"/>
<part name="PAD4" library="wirepad" library_urn="urn:adsk.eagle:library:412" deviceset="SMD2" device="" package3d_urn="urn:adsk.eagle:package:30839/1"/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="M1" gate="G$1" x="58.42" y="27.94"/>
<instance part="PAD1" gate="1" x="15.24" y="43.18"/>
<instance part="PAD2" gate="1" x="15.24" y="27.94"/>
<instance part="PAD3" gate="1" x="43.18" y="35.56" rot="R180"/>
<instance part="PAD4" gate="1" x="58.42" y="35.56" rot="R180"/>
</instances>
<busses>
</busses>
<nets>
<net name="N$1" class="0">
<segment>
<pinref part="M1" gate="G$1" pin="GATE"/>
<pinref part="PAD1" gate="1" pin="P"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="M1" gate="G$1" pin="SOURCE"/>
<pinref part="PAD2" gate="1" pin="P"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="M1" gate="G$1" pin="DRAIN"/>
<pinref part="PAD3" gate="1" pin="P"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
<compatibility>
<note version="8.2" severity="warning">
Since Version 8.2, EAGLE supports online libraries. The ids
of those online libraries will not be understood (or retained)
with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports URNs for individual library
assets (packages, symbols, and devices). The URNs of those assets
will not be understood (or retained) with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports the association of 3D packages
with devices in libraries, schematics, and board files. Those 3D
packages will not be understood (or retained) with this version.
</note>
</compatibility>
</eagle>
