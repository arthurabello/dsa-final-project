export default function(json, debounce) {
    const width = 800
    const height = width - 100
    const margin = 1

    json = json.map(e => ({
        name: e[0],
        weight: e[3]
    }))

    let maxWeight = 0

    for (let e of json) {
        maxWeight = Math.max(maxWeight, e.weight)
    }

    const tooltip = d3.select("#bubble")
        .append("div")
        .attr("id", "tooltip")
        .style("opacity", 0)

    function mouseover(ev) {
        tooltip.style("opacity", 1)
        d3.select(this)
            .style("stroke", "#212123")
    }
    function mousemove(ev, data) {
        tooltip
            .html(`"${data.name}": ${data.weight} occurrences`)
            .style("left", ev.clientX + 20 + "px")
            .style("top", ev.clientY + "px")
    }
    function mouseleave(d) {
        tooltip.style("opacity", 0)
        d3.select(this)
            .style("stroke", "none")
    }

    const color = d3.scaleLinear()
        .domain([0, maxWeight])
        .range(["lightskyblue", "tomato"])
    
    const svg = d3.select("#bubble").append("svg")
        .attr("width", width)
        .attr("height", height)

    const g = svg.append("g")

    const circles = g
        .selectAll("circle")
        .data(json)
        .enter()
        .filter(d => d.weight > 1)
        .append("circle")
        .attr("cx", width >> 1)
        .attr("cy", height >> 1)
        .attr("r", d => Math.sqrt(d.weight * 10))
        .attr("fill", d => color(d.weight))
        .on("mouseover", mouseover)
        .on("mouseleave", mouseleave)
        .on("mousemove", mousemove)

    const labels = g
        .selectAll("text")
        .data(json)
        .enter()
        .filter(d => d.weight > 1)
        .append("text")
        .attr("x", width >> 1)
        .attr("y", height >> 1)
        .attr("text-anchor", "middle")
        .attr("visibility", d => d.weight > 10? "visible" : "hidden")
        .text(d => d.name)

    function ticked() {
        circles.attr("cx", d => d.x).attr("cy", d => d.y)
        labels.attr("x", d => d.x).attr("y", d => d.y)
    }

    const forceStrength = 0.5

    function charge(d) {
        return -forceStrength * d.weight * 15;
    }

    const simulation = d3.forceSimulation()
        .force("x", d3.forceX().strength(forceStrength).x(0))
        .force("y", d3.forceY().strength(forceStrength).y(0))
        .force("charge", d3.forceManyBody().strength(charge))
        .on("tick", ticked)
        .alphaMin(.2)

    simulation.nodes(json)

    const updateScale = debounce(k => {
        g.selectAll("text")
            .attr("font-size", d => 15 / k)
            .attr("visibility", d => d.weight > 10 / k? "visible" : "hidden")

    }, 100)

    const zoom = d3.zoom().scaleExtent([.1, 10]).on("zoom", onZoom)
    function onZoom(ev) {
        g.attr("transform", ev.transform)
        updateScale(ev.transform.k)
    }

    svg.call(zoom)
    svg.call(zoom.transform, d3.zoomIdentity.translate(width >> 1, height >> 1))
}
