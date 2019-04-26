class Pelicula {

	//1) Constructor
	constructor(t, d, e, p, v){
		this.Titulo = t
		this.Descripcion = d
		this.Estreno = e
		this.Poster = (p != null) ? p : "http://www.seagarden.cl/home/images/contenido-no-disponible.jpg" 
		this.Trailer = v
	}

	//2) Metodos de Instancia
	Mostrar(){
		document.write(`
			<img src="${this.Poster}" width="150">
			<h1>${this.Titulo} (${this.Estreno})</h1>
			<p>${this.Descripcion}</p>
			<a href="${this.Trailer}" target="_blank">Reproducir Trailer</a>
			<hr>
		`)
	}

	Imprimir(area){

		let ficha = document.querySelector(".pelicula").cloneNode(true)

		ficha.querySelector("h4").innerText = this.Titulo
		ficha.querySelector("p").innerText = this.Estreno
		ficha.querySelector("img").src = this.Poster

		ficha.querySelector("a").onclick = () => {

			//window.alert(`Usted quiere ver: ${this.Trailer}`)
			document.querySelector("#playMovie iframe").src = `https://youtube.com/embed/${this.Trailer}`

		}

		ficha.classList.remove("hide")

		console.log(ficha)

		document.querySelector(area).appendChild(ficha)

	}

	//3) Metodos de Clase (ó Método Estático)
	static parse(obj){

		return new Pelicula(obj.Titulo, obj.Descripcion, obj.Estreno, obj.Poster, obj.Trailer)

	}
}